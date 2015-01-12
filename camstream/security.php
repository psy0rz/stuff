<?
	/*
		$Log: security.php,v $
		Revision 1.2  2005/03/24 19:21:50  psy
		*** empty log message ***

		Revision 1.1  2005/03/22 17:43:15  psy
		eerste commit
		
		Revision 1.3  2004/10/18 13:29:36  psy0rz
		*** empty log message ***
		
		Revision 1.2  2004/08/26 15:24:46  psy0rz
		added logs
		
	*/

	//created               20020310        Edwin Eefting           edwin at bit dot nl
    //edited                20020415        Edwin Eefting           edwin at bit dot nl
	//edited 		20020422	Edwin Eefting		edwin at bit dot nl
	//edited 		20020708	Edwin Eefting		edwin at bit dot nl

        /* v1.0 Initial release 
	 * v1.1 Now uses GLOBALS to find all the globals, doesn't check for magic_quotes_runtime anymore.
	 * v1.2 Checks if ip-adress of client "belongs" to this session. (prevent session key stealing)
	 * v1.3 Now doesn't filter sessiondata for html.
	 * v1.4 Added some reference variables like $G and $_GET to $HTTP_GET_VARS
	 *
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
	 * General security library. Include this in all your scripts 
	 * (or in common.php) to enhance security. This library
	 * performs extra checks and filtering, so it may break stuff!
	 * If it does, adjust your scripts, NOT this one. (it will become
	 * useless if you do)
	 *
	 * It will filter all userinput data (GPC), as well as other variables that
	 * could be controlled by the user (_SERVER, etc.)
	 * To exclude a certain "special" variable from HTML filtering
	 * insert a $Sec_HtmlSkip['varname']=1 before you include this script.
	 * This would mean that $_GET[varname], $_POST[varname] etc..etc.. are 
	 * excluded from filtering.
	 *
	 */

	#some general security settings checks

	if (!get_magic_quotes_gpc())
		SecError("magic_quotes_gpc not enabled!",
			 "Enable this setting in your php.ini or add 
			 <pre>php_flag magic_quotes_gpc on</pre> to your .htaccess");

	# we don't check for this , but if you want to be sure your data that comes FROM the database
	# and Exec() calls is also "safe" (e.g. escaped), you should enable this.
	 
/*	if (!get_magic_quotes_runtime())
		SecError("magic_quotes_runtime not enabled!",
			 "Enable this setting in your php.ini or add
			  <pre>php_flag magic_quotes_runtime on</pre> to your .htaccess");
*/	
			 
	if (ini_get('register_globals'))
		SecError("register_globals is enabled! (or your php version is < 4.x)",
			 "Disable this setting in your php.ini or add
			  <pre>php_flag register_globals off</pre> to your .htaccess");

	#check if the host addres belongs to this session
	#(to prevent session key stealing)
	#session_start();
	if (!isset($HTTP_SESSION_VARS[Sec_Host])){

		$HTTP_SESSION_VARS[Sec_Host]=$HTTP_SERVER_VARS[REMOTE_ADDR];
	}
	else
	{
		if ($HTTP_SESSION_VARS[Sec_Host] != $HTTP_SERVER_VARS[REMOTE_ADDR])
		{
			?>
				<html>
				<head>
					<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
				</head>					
				<body>
				<h1>Security problem</h1>
				Your IP-adress has changed from 
				<i>"<?=$HTTP_SESSION_VARS[Sec_Host]?>"</i> to <i>"<?=$HTTP_SERVER_VARS[REMOTE_ADDR]?>"</i>
				since the last time you loaded this page. <br>
				Please refresh this page and login again when needed.<br>
				<br>
				The administrator has been informed of this problem.<br>
				<!-- You wouldn't try to steal the session key, would you? :) -->
				</body></html>
				<?                      
				$HTTP_SESSION_VARS = array();
			session_destroy();
			session_unset();
			exit;
		}
	}
		
	#filter all get/post/cookie data. (execpt those in Sec_HtmlSkip[...])
	#(we also could do add_slashes here, but because of performance issues we
	# let magic_quotes handle this)
	
	#find out all the global arrays (like _GET , _POST etc..)
	foreach ($GLOBALS as $arrayname => $arrayref)
	{
		#is it really an array?
		if (is_array($arrayref) 
		&& ($arrayname != "GLOBALS") 
		&& ($arrayname != "HTTP_SESSION_VARS")
		&& ($arrayname != "_SESSION"))
		{
			#echo "filtering $arrayname<br>";
			#traverse the array (eg. _GET[bla], _GET[anotherone], ...) 
			foreach ($arrayref as $key => $value)
			{
				#remove html tags
				if (!isset($Sec_HtmlSkip[$key]))
				{
					${$arrayname}[$key] = str_replace("<", "&lt;", ${$arrayname}[$key]);
					${$arrayname}[$key] = str_replace(">", "&gt;", ${$arrayname}[$key]);
				}
			}
		}
	}	
	
	#make some shortnamed references to common used vars:
	$G=&$HTTP_GET_VARS;
	$P=&$HTTP_POST_VARS;
	$S=&$HTTP_SESSION_VARS;
	#also create the "new" variablenames if we're still using an older version of PHP.
	#(these aren't superglobals though)
	if (!isset($_GET)) 
		$_GET=&$HTTP_GET_VARS;
	if (!isset($_POST))
		$_POST=&$HTTP_POST_VARS;
	if (!isset($_SESSION))
		$_SESSION=&$HTTP_SESSION_VARS;

	#lame function to display security errors and exit
	function SecError($type,$msg)
	{	
		?>
			<h3>Internal security error - <?=$type?></h3>
			<p>
				<?=$msg?>
			</p>
			<p>
				This message is intended for the 
				<a href="mailto:<?=$_SERVER["SERVER_ADMIN"]?>" >webmaster</a>
				of this site. 
				
			</p>
		<?
		
		exit;
	}

	//these are just some helper functions:

	//Remove all slashes from a 2d array
	//$data should be a reference to a 2d array like this:
	//$data[rownr][columnname]="....";
	//Strip2dSlashes(&$data);
	function Strip2dSlashes($data)
	{
		//remove all slashes 
	        if (!is_array($data)) return;
		foreach($data as $rownr => $rowdata)
		{
			foreach($rowdata as $key => $value)
			{
				$data[$rownr][$key]=stripslashes($value);
			}
		}
	}

	//Removes all slashes from a array.
	//$data should be a reference to a 1d array like this:
	//$data[columnname]="....";
	// Strip1dSlashes(&$data);
	function Strip1dSlashes($data)
	{
		if (!is_array($data)) return;
		foreach($data as $key => $value)
		{
			$data[$key]=stripslashes($value);
		}
	}
	
?>
