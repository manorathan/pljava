/*
 * This file contains software that has been made available under The BSD
 * license. Use and distribution hereof are subject to the restrictions set
 * forth therein.
 * 
 * Copyright (c) 2004 TADA AB - Taby Sweden
 * All Rights Reserved
 */
package org.postgresql.pljava.jdbc;

import java.sql.SQLException;

/**
 * @author Thomas Hallgren
 */

public class SPINamedSavepoint extends SPISavepoint
{
	private final String m_name;

	/**
	 * Creates an named savepoint
	 */
	public SPINamedSavepoint(String name)
	{
		m_name = name;
	}

	public int getSavepointId()
	throws SQLException
	{
		throw new SQLException("ID not generated for savepoint");
	}

	public String getSavepointName()
	{
		return m_name;
	}

	String getSPIName()
	{
		return m_name;
	}
}
