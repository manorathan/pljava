/*
 * Copyright (c) 2003, 2004 TADA AB - Taby Sweden
 * Distributed under the terms shown in the file COPYRIGHT.
 * 
 * @author Thomas Hallgren
 */
#include "pljava/MemoryContext.h"

/* Single linked list of callback definitions. Each containing
 * a function and a client data pointer.
 */
struct _MctxCBLink;
typedef struct _MctxCBLink MctxCBLink;

struct _MctxCBLink
{
	MctxCBLink*   next;
	EndOfScopeCB  callback;
};

/* Extended version of the MemoryContextMethods structure that holds on
 * to the original structure and a callback chain.
 */
typedef struct {
	MemoryContextMethods  methods;
	MemoryContextMethods* prev;
	void*                 clientData;
	MctxCBLink*           cbChain;
	FreeCB                freeCB;
} ExtendedCtxMethods;

MemoryContext returnValueContext;

/**
 * Calls all user defined callbacks with their associated clientData as the
 * first argument and true as the second. Restores the original methods and finally
 * calls the original delete function for the MemoryContext.
 */
static void mctxDelete(MemoryContext ctx)
{
	ExtendedCtxMethods* exm = (ExtendedCtxMethods*)ctx->methods;
	MctxCBLink* cbs = exm->cbChain;
	while(cbs != 0)
	{
		MctxCBLink* nxt = cbs->next;
		(*cbs->callback)(ctx, true);
		pfree(cbs);
		cbs = nxt;
	}
	ctx->methods = exm->prev;
	pfree(exm);
	(*ctx->methods->delete)(ctx);
}

/**
 * Calls all user defined callbacks with their associated clientData as the
 * first argument and false as the second. Finally calls the original reset
 * function for the MemoryContext.
 */
static void mctxReset(MemoryContext ctx)
{
	ExtendedCtxMethods* exm = (ExtendedCtxMethods*)ctx->methods;
	MctxCBLink* cbs = exm->cbChain;
	while(cbs != 0)
	{
		(*cbs->callback)(ctx, false);
		cbs = cbs->next;
	}
	(*exm->prev->reset)(ctx);
}

static void mctxFree(MemoryContext ctx, void* deadData)
{
	ExtendedCtxMethods* exm = (ExtendedCtxMethods*)ctx->methods;
	(*exm->freeCB)(ctx, deadData);
	(*exm->prev->free_p)(ctx, deadData);
}

static void* parentContextAlloc(MemoryContext ctx, size_t size)
{
	MemoryContext pctx = ctx->parent;
	if(pctx == 0)
		elog(ERROR, "TopMemoryContext cannot be callback enhanced");
	return MemoryContextAlloc(pctx, size);
}

/**
 * Ensures that the given context has an extended MemoryContextMethods struct
 * capable of holding on to user defined callbacks.
 */
static ExtendedCtxMethods* MemoryContext_ensureCallbackCapability(MemoryContext ctx)
{
	ExtendedCtxMethods* exm;
	MemoryContextMethods* methods = ctx->methods;

	if(methods->reset == mctxReset)
		exm = (ExtendedCtxMethods*)methods;
	else
	{
		exm = (ExtendedCtxMethods*)parentContextAlloc(ctx, sizeof(ExtendedCtxMethods));
		memcpy(exm, methods, sizeof(MemoryContextMethods));
		exm->prev = methods;
		exm->cbChain = 0;
		exm->clientData = 0;
		exm->freeCB = 0;
		exm->methods.delete = mctxDelete;
		exm->methods.reset  = mctxReset;
		ctx->methods = (MemoryContextMethods*)exm;
	}
	return exm;
}

/**
 * Returns true if the MemoryContext has callback capabilities installed.
 */
bool MemoryContext_hasCallbackCapability(MemoryContext ctx)
{
	return ctx->methods->reset == mctxReset;
}

/**
 * Adds an end-of-scope callback from a MemoryContext.
 *
 * @param ctx
 * 		The context where the callback is registered
 * @param func
 *      The callback function that will be called when the context is
 *      either reset or deleted.
 */     
void MemoryContext_addEndOfScopeCB(MemoryContext ctx, EndOfScopeCB func)
{
	ExtendedCtxMethods* exm = MemoryContext_ensureCallbackCapability(ctx);
	MctxCBLink* link = (MctxCBLink*)parentContextAlloc(ctx, sizeof(MctxCBLink));
	link->callback = func;
	link->next = exm->cbChain;
	exm->cbChain = link;
}

/**
 * Returns the current clientData value.
 */
void* MemoryContext_getClientData(MemoryContext ctx)
{
	MemoryContextMethods* methods = ctx->methods;
	return (methods->reset == mctxReset)
		? ((ExtendedCtxMethods*)methods)->clientData
		: 0;
}

/**
 * Removes an end-of-scope callback from a MemoryContext. The callback is
 * identified using the function and the clientData.
 *
 * @param ctx
 * 		The context where the callback is registered.
 * @param func
 *      The callback function.
 */
void MemoryContext_removeEndOfScopeCB(MemoryContext ctx, EndOfScopeCB func)
{
	MemoryContextMethods* methods = ctx->methods;

	if(methods->reset == mctxReset)
	{
		ExtendedCtxMethods* exm = (ExtendedCtxMethods*)methods;
		MctxCBLink* prev = 0;
		MctxCBLink* curr = exm->cbChain;
		while(curr != 0)
		{
			if(curr->callback == func)
			{
				if(prev == 0)
					exm->cbChain = curr->next;
				else
					prev->next = curr->next;
				pfree(curr);
				break;
			}
			prev = curr;
			curr = curr->next;
		}
	}
}

/**
 * Assings a clientData value to be passed to the end-of-scope callbacks
 * for this context.
 */
void MemoryContext_setClientData(MemoryContext ctx, void* clientData)
{
	MemoryContext_ensureCallbackCapability(ctx)->clientData = clientData;
}

/**
 * Installs a callback that is called everytime a free is issued on the
 * given context. The old callback is returned. It is valid to pass NULL
 * as a callback.
 */
FreeCB MemoryContext_setFreeCB(MemoryContext ctx, FreeCB func)
{
	FreeCB old = 0;
	if(func == 0)
	{
		MemoryContextMethods* methods = ctx->methods;
		if(methods->free_p == mctxFree)
		{
			ExtendedCtxMethods* exm = (ExtendedCtxMethods*)methods;
			old = exm->freeCB;

			/* Disconnect callback caller
			 */
			exm->methods.free_p = exm->prev->free_p;
		}
	}
	else
	{
		ExtendedCtxMethods* exm = MemoryContext_ensureCallbackCapability(ctx);
		old = exm->freeCB;
		exm->freeCB = func;

		/* Install callback caller
		 */
		exm->methods.free_p = mctxFree;
	}
	return old;
}

MemoryContext MemoryContext_switchToReturnValueContext()
{
	return MemoryContextSwitchTo(returnValueContext);
}