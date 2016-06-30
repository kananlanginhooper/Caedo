<?php

if(isset($DontLoadCAEDO) && $DontLoadCAEDO) {
	// do nothing, the old framework is being loaded
}
else {
	// Just an easy place to store all the debuging code.
	// Allows for excel dumps of activites on a per request or global basis.
	$__StartSecond = 0;

	function GetMicroTime(){
		global $__StartSecond;
		
		// xdebug_break();
		$mtime = microtime();
		$arrTime = explode(" ", $mtime);
		if($__StartSecond == 0) {
			$__StartSecond = $arrTime[1];
		}
		$mtime = $arrTime[1] + ($arrTime[0] - $__StartSecond);
		return $mtime;
	}
	$__TAGS = array();

	function convert($size){
		$unit = array(
				'b',
				'kb',
				'mb',
				'gb',
				'tb',
				'pb' 
		);
		return @round($size / pow(1024, ($i = floor(log($size, 1024)))), 2) . ' ' . $unit[$i];
	}

	function __Tag($Tag){
		global $__TAGS, $IsLocal;
		if($IsLocal === null || $IsLocal || @$__ShowErrors || (isset($_SESSION['ShowErrors']) && $_SESSION['ShowErrors'])) {
			$__TAGS[] = array(
					'Tag' => $Tag,
					'Time' => GetMicroTime(),
					'Mem' => convert(memory_get_usage(true)) 
			);
		}
	}

	function __ReturnTags(){
		global $__TAGS;
		
		$ret = '';
		// xdebug_break();
		$StartTime = $LastTime = 0;
		foreach($__TAGS as $index => $arr) {
			
			if($LastTime == 0) {
				$Cume = $Lap = 0;
				$StartTime = $arr['Time'];
			}
			else {
				$Lap = to2(($arr['Time'] - $LastTime) * 1000); // *1000 converts to MS
				$Cume = to2(($arr['Time'] - $StartTime) * 1000); // *1000 converts to MS
			}
			
			$ret .= "{$arr['Tag']} in {$Lap}ms of {$Cume}ms @ {$arr['Mem']}<br /><br />";
			$LastTime = $arr['Time'];
		}
		return $ret;
	}
	
	__Tag('Start Framework');

	function xdebug_break_if_toggle_enabled(){
		if(isset(Site::$xdebug_toggle) && Site::$xdebug_toggle) xdebug_break();
	}

	function xdebug_break_toggle_Off(){
		if(isset(Site::$xdebug_toggle)) {
			Site::$xdebug_toggle = false;
		}
	}

	function xdebug_break_toggle_On(){
		if(isset(Site::$xdebug_toggle)) {
			Site::$xdebug_toggle = true;
		}
	}
	
	if($UsingComposer) {
		// setup Caedo Root Folder
		$CaedoRootFolder = $RootFolder . "##VENDOR/caedo/framework/";
	}
	else {
		// setup Caedo Root Folder
		$CaedoRootFolder = $RootFolder . "##CAEDO/framework/";
	}
	
	require_once $CaedoRootFolder . 'CoreClasses.inc';
	
	Site::$RootFolder = $RootFolder;
	Site::$CaedoConfigFolder = $RootFolder . '__CAEDO_CONFIG/';
	Site::$CaedoRootFolder = $CaedoRootFolder;
	
	//
	//
	//
	//
	//
	// AUTO LOAD
	//
	//
	//
	//
	//
	
	// Setup Class AutoLoader
	function autoload_SaveFileToStaticIncludes($ClassFileName){
		global $RootFolder;
		$IncludeFileName = $RootFolder . "__STATIC_INCLUDES" . $_SERVER['SCRIPT_NAME'] . '.inc';
		
		if(!file_exists(dirname($IncludeFileName))) {
			mkdir(dirname($IncludeFileName), 0777, true);
		}
		
		// if the include file doesn't exits, create it
		if(!file_exists($IncludeFileName)) {
			file_put_contents($IncludeFileName, "<?php\n");
		}
		
		// append to the include file this class. We were including everything before, this should be a shorter list.
		$fh = fopen($IncludeFileName, 'a') or die("can't open file");
		$Escaped_RootFolder = str_replace("\\", "\\\\", $RootFolder);
		fwrite($fh, "include_once '$ClassFileName';\n");
		fclose($fh);
	}

	function autoload_SaveUseAsToStaticIncludes($Use, $As){
		global $RootFolder;
		$IncludeFileName = $RootFolder . "__STATIC_INCLUDES" . $_SERVER['SCRIPT_NAME'] . '.inc';
		
		if(!file_exists(dirname($IncludeFileName))) {
			mkdir(dirname($IncludeFileName), 0777, true);
		}
		
		// if the include file doesn't exits, create it
		if(!file_exists($IncludeFileName)) {
			file_put_contents($IncludeFileName, "<?php\n");
		}
		
		// append to the include file this class. We were including everything before, this should be a shorter list.
		$fh = fopen($IncludeFileName, 'a') or die("can't open file");
		$Escaped_RootFolder = str_replace("\\", "\\\\", $RootFolder);
		fwrite($fh, "use $Use as $As;\n");
		fclose($fh);
	}

	function FileNameUsingNamingConvention($BasePath, $ClassName, $FileNamingConvention){
		switch ($FileNamingConvention) {
			case 'caedo' :
				return $BasePath . '/' . $ClassName . '.class.inc';
				break;
			case 'composer' :
				return $BasePath . '/' . $ClassName . '.php';
				break;
			default :
				xdebug_break();
				die('Bad Filenaming convention');
				break;
		}
	}

	function autoload_recurse($BaseFolder, $pClassName, $arrClassFileName, $FileNamingConvention){
		$directories = glob($BaseFolder . '/*', GLOB_ONLYDIR);
		foreach($directories as $dir) {
			
			$filename = FileNameUsingNamingConvention($dir, $pClassName, $FileNamingConvention);
			
			$arrClassFileName[$filename] = $filename;
			$arrClassFileName = autoload_recurse($dir, $pClassName, $arrClassFileName, $FileNamingConvention);
		
		}
		
		$filename = FileNameUsingNamingConvention($BaseFolder, $pClassName, $FileNamingConvention);
		$arrClassFileName[$filename] = $filename;
		
		return $arrClassFileName;
	}

	function autoload($pClassName){
		global $RootFolder, $CaedoRootFolder;
		if(false && clsMemcache::$HasMemcache && clsMemcache::$CacheEnabled) {
			// use memcache to save a list of classes that should be included because they are often used with this code
			$MemCachePageClass = 'PageClasses__' . $_SERVER['SCRIPT_NAME'];
			$arrUsedClasses = CacheGet($MemCachePageClass);
			$arrUsedClasses[$pClassName] = true;
			CacheSet($MemCachePageClass, $arrUsedClasses);
		}
		
		if(class_exists($pClassName)) {
			// class is already in the current context, this shouldn't even be running
			return false;
		}
		
		// can't use memcache for this, need to be able to store the opcodes, not dynamicly add them
		// -- we don't actually need to read this file
		// if (file_exists($RootFolder."StaticIncludes/".$_SERVER['SCRIPT_NAME'])){
		// $arrFileLines = file($RootFolder."StaticIncludes/".$_SERVER['SCRIPT_NAME'],FILE_SKIP_EMPTY_LINES);
		// foreach($arrFileLines as $fileLine){
		// if(strpos($fileLine,'<')!==false){
		// $ArrIncludedFiles = substr($fileLine, strlen('include_once') ,-1);
		// }
		// }
		// }
		
		// deal with namespaces....
		if(strpos($pClassName, '\\')) {
			$arrClass = explode('\\', $pClassName);
			$arrClass = array_reverse($arrClass);
			$ClassName = $arrClass[0];
		}
		else {
			$ClassName = $pClassName;
		}
		
		// seach all modules to find the class
		// xdebug_break();
		$arrClassFileName = array();
		
		// all code folders are loaded just the same, if a file exists in local it will supersede the framework file, first file wins
		
		$CaedoRootFolder_noSlash = rtrim($CaedoRootFolder, "/");
		
		$arrClassFileName = autoload_recurse($CaedoRootFolder_noSlash, $ClassName, $arrClassFileName, 'caedo');
		// $arrClassFileName = autoload_recurse($RootFolder . '__vendors', $ClassName, $arrClassFileName, 'composer');
		$arrClassFileName = autoload_recurse($RootFolder . '__LOCAL_USER_CLASSES', $ClassName, $arrClassFileName, 'caedo');
		
		foreach($arrClassFileName as $ClassFileName) {
			if(file_exists($ClassFileName)) {
				
				// these two lines must be in this order, or if the include has dependencies, they will never auto load, and always be ondemand
				include_once $ClassFileName; // actually include the file
				autoload_SaveFileToStaticIncludes($ClassFileName); // add to the static include for next time
				                                                   
				// namespaces can be needed in some cases?
				if(false) {
					// deal with namespaces
					if($pClassName != $ClassName) {
						xdebug_break();
						eval("use $pClassName as $ClassName;");
						autoload_SaveUseAsToStaticIncludes($pClassName, $ClassName);
					}
				}
			}
		}
	}
	
	spl_autoload_register("autoload");
	
	//
	// !!!!
	//
	// this section really needs to be redone. Because the static includes are called here, we need to have thses functions loaded and ready just in case they are used
	// there are some date functions in here that need to be split out and make static on a date managment class
	// in fact all the functions in here need to be split out and made into different functions
	
	// Load in global functions
	require_once $CaedoRootFolder . "GlobalFunctions.inc";
	
	// Then run the Config starter, this should include a reference to '__CAEDO_CONFIG/LocalFunctions.inc'
	require_once $RootFolder . '__CAEDO_CONFIG/__config.Starter.inc';
	
	//
	//
	//
	//
	//
	// Basic Server setup
	// We need to identify what server URL we're on.
	//
	//
	//
	//
	//
	
	// this will return false if no match is found
	$SiteConfig = clsSiteConfigs::GetSiteConfig();
	
	$siteFound = false;
	
	// need to add URL redirect Authentication
	
	if($SiteConfig) {
		Site::setSite($SiteConfig);
		$siteFound = true;
	}
	
	// this is for direct calls to the server via IP address.
	// This is useful as a hack when using clustered servers. I have a few very specific pages that require websockets to run fast enough.
	// Once the socket server is started, I need to be sure that I am connecting to the server that is hosting that socket, and not just any server in the cluster.
	// For now, the websocket program itself authenticates the user, but we need to add code here, perhaps built on the session to verify that this user can indeed request that server config
	elseif(isset($_GET['Url']) && $_GET['Url'] && $SiteConfig && Site::$AllowURLOverride) {
		
		// if(strtolower($_SERVER["HTTP_HOST"]) != 'localhost') session_save_path('/tmp');
		// we need to start the session, there could be session data that is important.
		session_start();
		
		$SiteConfig = clsSiteConfigs::GetSiteConfig($_GET['Url']);
		if($SiteConfig) {
			Site::setSite($SiteConfig);
			$_SESSION['Url'] = strtolower($_GET['Url']);
			$siteFound = true;
		}
	}
	// this is for repeated calls to the server via IP address.
	elseif(isset($_SESSION['Url']) && $_SESSION['Url'] && $SiteConfig && Site::$AllowURLOverride) {
		
		// if(strtolower($_SERVER["HTTP_HOST"]) != 'localhost') session_save_path('/tmp');
		// we need to start the session, there could be session data that is important.
		session_start();
		
		$SiteConfig = clsSiteConfigs::GetSiteConfig($_SESSION['Url']);
		if($SiteConfig) {
			Site::setSite($SiteConfig);
			$_SESSION['Url'] = strtolower($_GET['Url']);
			$siteFound = true;
		}
	}
	
	if(!$siteFound) {
		xdebug_break();
		print 'Failed to find site!' . '<br />' . 'Host is: ' . $_SERVER["HTTP_HOST"] . '<br />' . 'SERVER_NAME is: ' . $_SERVER["SERVER_NAME"];
		LogError('Failed to find site!', 'Host is: ' . $_SERVER["HTTP_HOST"], 'SERVER_NAME is: ' . $_SERVER["SERVER_NAME"]);
		
		// the above code should work. For some reason it looks like Apache in beanstalk has an error and is passing the IP and port of the worker thread not the url
		// don't have time to fully debug this, so I'm going to use a param, and skip for now.
		header('HTTP/1.0 599 Server Configuration Error');
		exit();
	}
	
	// this is not the right way to do this, need to move all functions such as APC_GET into the APC class and make them static
	// $clsMemcache = new clsMemcache('n/a'); // we will need to load this again, down past config.site.inc
	
	// load common classes
	$IncludeFileName = $RootFolder . "__STATIC_INCLUDES" . $_SERVER['SCRIPT_NAME'] . '.inc';
	if(!file_exists($IncludeFileName)) {
		@touch($IncludeFileName);
		// put < ?php in the file
		@file_put_contents($IncludeFileName, "<?php\n");
	}
	else {
		include_once $IncludeFileName;
	}
	
	//
	//
	//
	//
	
	// --------------------- Error and Debug - Move as high as you can
	
	if(!function_exists('session_status')) {

		function session_active(){
			return defined('SID');
		}
	}
	else {

		function session_active(){
			return (session_status() == PHP_SESSION_ACTIVE);
		}
	}
	
	// Turn on all errors for the setup process
	
	// DebugBreak();
	function LogError(){
		$arrDebug = debug_backtrace();
		
		$str = 'Error: ' . $arrDebug[0]['file'] . ':' . $arrDebug[0]['line'] . " ";
		$args = func_get_args();
		$FunctionArgsAndValues = "";
		if(isset($args) && is_array($args) && count($args) > 0) {
			$FunctionArgsAndValues .= " || " . implode(" | ", func_get_args()) . " || ";
		}
		
		// if we're logging SQL, also log errors
		__log_error($args, $arrDebug[0]['file'], $arrDebug[0]['line'], $FunctionArgsAndValues);
		
		error_log($str . $FunctionArgsAndValues);
	}

	function LogPassThroughError(){
		$arrDebug = debug_backtrace();
		// DebugBreak();
		
		$str = 'Error: ' . $arrDebug[1]['file'] . ':' . $arrDebug[1]['line'] . " ";
		$args = func_get_args();
		$FunctionArgsAndValues = "";
		if(isset($args) && is_array($args) && count($args) > 0) {
			$FunctionArgsAndValues .= " || " . implode(" | ", func_get_args()) . " || ";
		}
		
		// if we're logging SQL, also log errors
		__log_error($args, $arrDebug[1]['file'], $arrDebug[1]['line'], $FunctionArgsAndValues);
		
		error_log($str . $FunctionArgsAndValues);
	}

	function __log_error($arrFunctionArgs, $File, $Line, $strFunctionArgs){
		// DebugBreak();
		if(function_exists('cacheGet')) {
			$Logging = Instance::$LoggingEnabled;
		}
		else {
			$Logging = false;
		}
		
		// if we're logging SQL, also log errors
		if($Logging) {
			$edErrorMessage0 = $edErrorMessage1 = $edErrorMessage2 = $edErrorMessage3 = '';
			if(isset($arrFunctionArgs) && is_array($arrFunctionArgs) && count($arrFunctionArgs) > 0) {
				if(isset($arrFunctionArgs[0])) $edErrorMessage0 = $arrFunctionArgs[0];
				if(isset($arrFunctionArgs[1])) $edErrorMessage1 = $arrFunctionArgs[1];
				if(isset($arrFunctionArgs[2])) $edErrorMessage2 = $arrFunctionArgs[2];
				if(isset($arrFunctionArgs[3])) $edErrorMessage3 = $arrFunctionArgs[3];
			}
			
			$sql = "insert into sys_errordebug (edSessionID, edTime, edErrorMessage0, edErrorMessage1, edErrorMessage2, edErrorMessage3, edFile, edLine, edAllMessages)
        VALUES ( '" . session_id() . "' ,'" . realtimesql('NE') . "' 
        ,'" . escape_string($edErrorMessage0) . "','" . escape_string($edErrorMessage1) . "','" . escape_string($edErrorMessage2) . "','" . escape_string($edErrorMessage3) . "'
        , '" . escape_string($File) . "', {$Line}, '" . escape_string($strFunctionArgs) . "')";
			sql_query($sql);
		}
	}
	// --------------------- Error and Debug
	
	// ---------------config setup for Site, ENV and DB
	// setup error reporting based on sessions
	// DebugBreak();
	if(@$__ShowErrors || (isset($_SESSION['ShowErrors']) && $_SESSION['ShowErrors'] == 'ALL')) {
		error_reporting(E_ALL);
	}
	elseif(isset($_SESSION['ShowErrors']) && $_SESSION['ShowErrors'] == 'ALLButNotice') {
		error_reporting(E_ERROR | E_WARNING | E_PARSE);
	}
	else {
		error_reporting(0);
	}
	
	$PublicDNSOfThisServer = Instance::$PublicDNS;
	
	ob_start();
	
	// ----------------------------- mysqli, new DB connection
	
	//
	function sql_multi_query($SQL){
		if(!DB::$dbh) {
			LogPassThroughError("No DB connection");
			DebugBreak();
		}
		elseif($SQL == '') {
			LogPassThroughError("Empty SQL Query");
			DebugBreak();
		}
		else {
			$res = DB::$mysqli_write->multi_query($SQL);
			if(isset($_GET) && isset($_GET['Debug'])) LogPassThroughError($SQL); // log if ?Debug is set
			if(!DB::$mysqli_write->error) {
				return $res;
			}
			else {
				LogPassThroughError("SQL ERROR Query", DB::$mysqli_write->error, $SQL);
				DebugBreak();
			}
		}
	}

	function sql_query_or_die($DieMessage){
		xdebug_break();
		die($DieMessage);
	}

	function sql_query($SQL, $unbuffered = false){
		global $RootFolder;
		
		// DebugBreak();
		// file_put_contents($RootFolder.'SQL.sql', $SQL.";\n", FILE_APPEND);
		
		if(!DB::$dbh) {
			xdebug_break();
			xdebug_break();
		}
		elseif($SQL == '') {
			// DebugBreak();
			LogPassThroughError("Empty SQL Query");
		}
		else {
			
			// check to see if logging is enabled.
			if(Instance::$LoggingEnabled && !strpos($SQL, 'sys_sqldebug') && !strpos($SQL, 'sys_memcachedebug') && !strpos($SQL, 'sys_errordebug')) {
				$arrDebug = debug_backtrace();
				sql_query("insert into sys_sqldebug (sqSessionID, sqTime, sqQuery, sqFile, sqLine) VALUES ('" . session_id() . "','" . realtimesql('NE') . "','" . escape_string($SQL) . "', '" . escape_string($arrDebug[0]['file']) . "', {$arrDebug[0]['line']})");
				$sqAuto = insert_id();
				$start = microtime(true);
			}
			
			// disable unbuffered for now
			// Run Query
			// if($unbuffered){
			// $res = DB::$mysqli->query($SQL, MYSQLI_USE_RESULT);
			// if(is_object($res) && is_a($res,'mysqli_result')){
			// $res->free_result(); // needed so an additional query can be run
			// }
			// }else{
			
			// check to see if this is a select query:
			if(strtoupper(substr($SQL, 0, 6)) == 'SELECT') { // will fail on select into
				$mysqli = DB::$mysqli_read;
			}
			else {
				$mysqli = DB::$mysqli_write;
			}
			$res = $mysqli->query($SQL);
			if(isset($_GET) && isset($_GET['Debug'])) LogPassThroughError($SQL); // log if ?Debug is set
			                                                                     
			// }
			                                                                     
			// check to see if logging is enabled.
			if(Instance::$LoggingEnabled && !strpos($SQL, 'sys_sqldebug') && !strpos($SQL, 'sys_memcachedebug') && !strpos($SQL, 'sys_errordebug')) {
				$End = microtime(true);
				$totalTime = $End - $start;
				if($totalTime > 700) DebugBreak();
				sql_query("update sys_sqldebug set sqExecTime = $totalTime where sqAuto = $sqAuto", true);
			}
			
			if(!$mysqli->error) {
				return $res; // worked
					             // return DB::$mysqli->use_result(); // worked
			}
			else {
				// nope
				// DB::$mysqli->error
				LogPassThroughError("SQL ERROR Query", $mysqli->error, $SQL);
				// DebugBreak();
			}
		}
	}

	function sql_query_datasets_only($SQL, $unbuffered = false){
		global $RootFolder;
		
		// file_put_contents($RootFolder.'SQL.sql', $SQL.";\n", FILE_APPEND);
		
		if(!DB::$dbh) {
			xdebug_break();
			xdebug_break();
			xdebug_break();
		}
		elseif($SQL == '') {
			DebugBreak();
			LogPassThroughError("Empty SQL Query");
		}
		else {
			
			// check to see if logging is enabled.
			if(Instance::$LoggingEnabled && !strpos($SQL, 'sys_sqldebug') && !strpos($SQL, 'sys_memcachedebug') && !strpos($SQL, 'sys_errordebug')) {
				$arrDebug = debug_backtrace();
				sql_query("insert into sys_sqldebug (sqSessionID, sqTime, sqQuery, sqFile, sqLine) VALUES ('" . session_id() . "','" . realtimesql('NE') . "','" . escape_string($SQL) . "', '" . escape_string($arrDebug[0]['file']) . "', {$arrDebug[0]['line']})");
				$sqAuto = insert_id();
				$start = microtime(true);
			}
			
			// disable unbuffered for now
			// Run Query
			// if($unbuffered){
			// $res = DB::$mysqli->query($SQL, MYSQLI_USE_RESULT);
			// if(is_object($res) && is_a($res,'mysqli_result')){
			// $res->free_result(); // needed so an additional query can be run
			// }
			// }else{
			
			// check to see if this is a select query:
			if(strtoupper(substr($SQL, 0, 6)) == 'SELECT') { // will fail on select into
				$mysqli = DB::$mysqli_read;
			}
			else {
				$mysqli = DB::$mysqli_write;
			}
			
			// DebugBreak();
			$res = $mysqli->query($SQL);
			
			if(isset($_GET) && isset($_GET['Debug'])) LogPassThroughError($SQL); // log if ?Debug is set
			                                                                     
			// }
			                                                                     
			// check to see if logging is enabled.
			if(Instance::$LoggingEnabled && !strpos($SQL, 'sys_sqldebug') && !strpos($SQL, 'sys_memcachedebug') && !strpos($SQL, 'sys_errordebug')) {
				$End = microtime(true);
				$totalTime = $End - $start;
				if($totalTime > 700) DebugBreak();
				sql_query("update sys_sqldebug set sqExecTime = $totalTime where sqAuto = $sqAuto", true);
			}
			
			if(!$mysqli->error) {
				return $res; // worked
					             // return DB::$mysqli->use_result(); // worked
			}
			else {
				// nope
				// DB::$mysqli->error
				LogPassThroughError("SQL ERROR Query", $mysqli->error, $SQL);
				// DebugBreak();
			}
		}
	}
	
	// mysql_ fetch_assoc
	function fetch_assoc($res){
		if(is_object($res)) {
			return $res->fetch_assoc();
		}
		else {
			LogPassThroughError("SQL ERROR fetch_array");
			return false;
		}
	}
	
	// mysql _real_escape_string
	function escape_array($arrToEscape){
		if(!is_array($arrToEscape)) {
			LogPassThroughError("escape_array passed an value");
			DebugBreak();
			$arrToEscape = print_r($arrToEscape, true);
		}
		
		if(!DB::$dbh) {
			LogPassThroughError("escape_array without DB connection");
			return $arrToEscape;
		}
		else {
			foreach($arrToEscape as &$Value) {
				$Value = real_escape_string($Value);
			}
			return $arrToEscape;
		}
	}

	function escape_string($StringToEscape){
		if(is_array($StringToEscape)) {
			LogPassThroughError("real_escape_string passed an array");
			DebugBreak();
			$StringToEscape = print_r($StringToEscape, true);
		}
		
		if(!DB::$dbh) {
			LogPassThroughError("real_escape_string without DB connection");
			return $StringToEscape;
		}
		else {
			return real_escape_string($StringToEscape);
		}
	}

	function real_escape_string($StringToEscape){
		return DB::$mysqli_read->real_escape_string($StringToEscape);
	}
	
	// mysql_ fetch_array
	function sql_fetch_array($res){
		fetch_array($res);
	}

	function fetch_array($res){
		if(is_object($res)) {
			return $res->fetch_array();
		}
		else {
			LogPassThroughError("SQL ERROR fetch_array");
			return false;
		}
	}
	
	// mysql_ fetch_row
	function fetch_row($res){
		if(is_object($res)) {
			return $res->fetch_row();
		}
		else {
			LogPassThroughError("SQL ERROR Fetch_Row");
			return false;
		}
	}
	
	// mysql _result
	function sql_result($res, $row, $field){
		if($res->num_rows == 0) return null;
		$res->data_seek($row);
		$ceva = fetch_assoc($res);
		$rasp = $ceva[$field];
		return $rasp;
	}
	
	// mysql _fetch_field()
	function fetch_field($res){
		if(is_object($res)) {
			return $res->fetch_field();
		}
		else {
			LogPassThroughError("SQL ERROR fetch_field");
			return false;
		}
	}
	
	// mysql _num_rows()
	function sql_num_rows($res){
		return num_rows($res);
	}

	function num_rows($res){
		if(is_object($res)) {
			return $res->num_rows;
		}
		else {
			// LogPassThroughError("SQL ERROR num_rows"); // not usually an error, just no result, so no rows returned
			return 0;
		}
	}
	
	// mysql _num_rows()
	function sql_num_fields($res){
		return num_fields($res);
	}

	function num_fields($res){
		if(is_object($res)) {
			return $res->field_count;
		}
		else {
			// LogPassThroughError("SQL ERROR num_rows"); // not usually an error, just no result, so no rows returned
			return 0;
		}
	}
	
	// mysql _num_rows()
	function sql_fetch_fields($res){
		return fetch_fields($res);
	}

	function fetch_fields($res){
		if(is_object($res)) {
			return $res->fetch_fields();
		}
		else {
			// LogPassThroughError("SQL ERROR num_rows"); // not usually an error, just no result, so no rows returned
			return 0;
		}
	}
	
	// mysql _affected_rows()
	function sql_affected_rows(){
		return affected_rows();
	}

	function affected_rows($res = null){
		if($res) {
			if(is_object($res)) {
				return $res->affected_rows();
			}
			else {
				// LogPassThroughError("SQL ERROR affected_rows");
				return false;
			}
		}
		else {
			return mysqli_affected_rows(DB::$mysqli_write);
		}
	}
	
	// mysql _free_result()
	function sql_free_result($res){
		return free_result($res);
	}

	function free_result($res){
		if(is_object($res)) {
			return $res->free_result();
		}
		else {
			LogPassThroughError("SQL ERROR free_result");
			return false;
		}
	}
	
	// mysql_ insert_id
	function sql_insert_id(){
		return insert_id();
	}

	function insert_id(){
		if(DB::$dbh) {
			return DB::$mysqli_write->insert_id;
		}
		else {
			LogPassThroughError("SQL ERROR affected_rows");
			return false;
		}
	}
	// ----------------------------- mysqli, new DB connection
	
	// save for later, Server Identity, eg
	DB::$mysqli_read = DB::$mysqli_write = DB::$dbh = false;
	Site::$HasDatabase = false;
	
	try {
		if(Site::$ShouldHaveDatabase) {
			
			// Setup Write Connection
			if(!clsDatabaseConnections::GetConnection(Site::$Env, false)) xdebug_break();
			DB::$connectionDetails_read = DB::$connectionDetails_write = clsDatabaseConnections::GetConnection(Site::$Env, false);
			if(!DB::$connectionDetails_write) {
				xdebug_break();
				// DebugBreak();
				$Error = 'Could not connect. Error #3381';
				LogPassThroughError("COULD NOT FIND CONNECTION", Site::$Env);
			}
			else {
				DB::$mysqli_write = @new mysqli(DB::$connectionDetails_write->HostURL, DB::$connectionDetails_write->UserName, DB::$connectionDetails_write->Password, Site::$WriteDatabase);
				if(!DB::$mysqli_write || (DB::$mysqli_write && DB::$mysqli_write->connect_error)) {
					xdebug_break();
					// die('Connect Error: ' . DB::$mysqli->connect_error);
					// can't use this: DB::$mysqli->connect_error; DB::$mysqli is not defined
					LogPassThroughError("SQL CONNECTION ERROR", DB::$connectionDetails_write->HostURL, DB::$connectionDetails_write->UserName, DB::$connectionDetails_write->Password, Site::$WriteDatabase);
					DB::$dbh = false;
					$Error = 'Could not connect. Error #3382';
				}
				else {
					DB::$dbh = true;
					Site::$HasDatabase = true;
				}
			}
			
			$UseReadOnly = (Site::$ReadDatabase);
			if($UseReadOnly) {
				// Setup Write Connection
				DB::$connectionDetails_read = clsDatabaseConnections::GetConnection(Site::$Env, true);
				if(!DB::$connectionDetails_read) {
					DB::$mysqli_read = DB::$mysqli_write;
					xdebug_break();
					$Error = 'Could not connect. Error #3381';
					LogPassThroughError("COULD NOT FIND CONNECTION", Site::$Env);
				}
				else {
					DB::$mysqli_read = @new mysqli(DB::$connectionDetails_read->HostURL, DB::$connectionDetails_read->UserName, DB::$connectionDetails_read->Password, Site::$ReadDatabase);
					if(!DB::$mysqli_read || (DB::$mysqli_read && DB::$mysqli_read->connect_error)) {
						LogPassThroughError("SQL CONNECTION ERROR", DB::$connectionDetails_read->HostURL, DB::$connectionDetails_read->UserName, DB::$connectionDetails_read->Password, Site::$ReadDatabase);
						DB::$mysqli_read = DB::$mysqli_write;
						DebugBreak();
					}
				}
			}
			else {
				DB::$mysqli_read = DB::$mysqli_write;
			}
		}
		
		unset($DBConnection); // distroy this so it can't be accessed later. We have a connection, that's all that's needed
	}
	catch ( Exception $e ) {
		LogPassThroughError("CONNECTION Exception", $e->message);
		DB::$dbh = false;
		Site::$HasDatabase = false;
		$Error = 'Could not connect. Error #3383';
	}
	// }
	// DBConnect();
	
	$__Instance = new Instance(); // Save so GC doesn't steal it
	
	ob_end_clean(); // just in case one of these commands dumped out a space or something
	
	if(!Site::$ShouldHaveDatabase || (Site::$ShouldHaveDatabase && Site::$HasDatabase)) {
		
		// Everything is setup, time to start the session
		if(!session_active()) session_start(); // session will already be active if we're running on localhost
		                                       
		// not sure what this does // $_SESSION['SiteProfile'] = Site::$Profile;
		                                       
		// activate logging ability for memcache
		Instance::$SQLDebug_MemcacheStarted = (false);
	}
	
	elseif(!@$RunningHealthCheck) {
		xdebug_break();
		
		include Site::$RootFolder . '__CAEDO_CONFIG/ErrorPages/DatabaseDown.inc';
		die('');
	}
	
	// Security CleanUp
	// Remove Database info, connections strings, and other setup info that might cause security holes and aren't needed anyway
	
	clsDatabaseConnections::SecurityCleanUp();
	clsSiteConfigs::SecurityCleanUp();
	
	__Tag('framework Loaded');
}
?>