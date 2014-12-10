<? 
/* 
 * (C) 2001 Edwin Eefting (edwin@bit.nl)
 *                              __________________
 *                             /\ ___/          
 * http://www.bit.nl          /- \ _/  Business Internet Trends BV
 *                           /--- \/           __________________
 *                                                      
 * Released under GPL - Look at http://www.gnu.org/licenses/gpl.html
 *
 * sig=p$y cr3@73|) 7][!$ /\/\@$73rp!3c3
 *
 * This thing should generize database access and provide some
 * usefull functions, so that we don't have to use SQL statements
 * anywhere else in this project.
 * 
 * Most of the time you probably want to use GetRow and SetRow to
 * manupilate rows that work with a column names 'id' as primary key
 *
 */

require_once("security.php");
require_once("db.inc.php");

//returns last error text
function db_GetError()
{
	return(mysql_error());
}

//connects to the db.
//ret=false when failed.
function db_Conn()
{
	global $dbms_hn;
	global $dbms_un;
	global $dbms_pw;
	global $db_name;
	require( "db.inc.php");
	$link= mysql_connect($dbms_hn, $dbms_un, $dbms_pw);
	mysql_query("SET CHARACTER SET 'utf8'", $link);
	if ($link)
	{
		return (mysql_select_db($db_name,$link));
		//return 1;
	}
	return (false);
}
 
//just perform a query (to stay portable)
// Returns the resultresource
function db_doQuery($query)
{
	return(mysql_query($query));
}

//fetch all results from query resource,
//returns data as 2 dimensional array:
// $result[$row_nr][$col_name]
function db_Fetch($resource)
{
	$data=array();
	while ($row=db_FetchRow($resource))
	{
		$data[]=$row;	
	}	
	return $data;
}

//fetch ONE row and return it as hash array.
function db_FetchRow($resource)
{
	if ($resource)
		return(mysql_fetch_assoc($resource));
	else
		return 0;
}

//to keep things simple:
function db_QueryFetch($query)
{
	return(db_Fetch(db_doQuery($query)));
}

function db_QueryFetchHash($query)
{
	$res=db_Fetch(db_doQuery($query));
	foreach ($res as $data)
	{
		$ret[$data['id']]=$data;
	}
	return ($ret);
}


function db_QueryFetchRow($query)
{
	return(db_FetchRow(db_doQuery($query)));
}

//free a query resource (saving mem.)
function db_Free($resource)
{
	return(mysql_free_result($resource));
}

//return last autoincrement ID
function db_InsertId()
{
	return(mysql_insert_id());
}

//Gets the data of row with the specified ID from specified table
//rets hash array
//e.g.:
//$data=db_GetRow("users",1)
//$data['name'] contains name
//$data['blah'] contains...etc..
function db_GetRow($table,$id)
{
	global $db_name;
	$cols=db_FetchRow(db_doQuery("SELECT * FROM `$db_name`.$table WHERE id='$id'"));
	return $cols;
}

//Sets the specified row in the table with the data from the
//hasharray data.
//The hash array should only contain entries that are valid in the 
//table. If id==0, a new row will be added. If data is not specified
//the row ID will be deleted
//e.g:
//change row 1:
//	$data['name']="edwin"
//	$data['level']="5"
//	db_SetRow("users",1,$data);
//add a row:
//	$data['blah']="newthing"
//	$data['mekker']="asdf"
//	db_SetRow("sometable",0,$data);
//del row 22:
//	db_SetRow("sometable",22);
function db_SetRow($table,$id,$data=false)
{
	global $db_name;
	
	

	//create dataset
	$set="";
	if ($data != false)
	{
		foreach($data as $col => $value)
		{	
			$set.="`$col`=\"$value\", ";
		}
		$set=rtrim($set,", ");
	}
	
	//what to do?
	//insert..
	if ($id==0)
	{
		$query="INSERT INTO `$db_name`.`$table` SET $set";
		//die( $query );
	}
	else
	{
		//delete...
		if ($data==false)
		{
			$query="DELETE FROM `$db_name`.`$table` WHERE `id`=\"$id\"";
		}
		//update...
		else
		{
			$query="UPDATE `$db_name`.`$table` SET $set WHERE `id`=\"$id\"";
		}
	}
	return(db_doQuery("$query"));
}

//alway connect to the db if we use this php module..

db_Conn() or die ("Can't connect to database!");

?>
