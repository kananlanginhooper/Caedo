<?php

// Globals that we use: $RootFolder, $IsLocal

// $RootFolder is the base execution directory, in linux or Windows
// $IsLocal tells us if we are operating on a localhost machine, for debugging. It is used to turn off some features, like emails.

// debug
function Debug_Flag(){
	if(isset($_GET['Debug'])) DebugBreak();
}

// The basic idea here is that we need a way to store data and access it locally from memory
// We don't have enough memory to store everything. We also have data sets that are large, and we need a way to have a partial representation of them.
// We may need some parts for a little while and then move away from needing them.
// all data can be read only.
// What we're going to do is use a kind of paging. We pull from the DB, into php and parse.
// If memcache is available we save the data there and unset it from the server.
// if memcache is not available we page it in directly from the DB in chunks.
// the memcache and db paging need to be transparent to the users of dataReporting, it needs to just work, and never fail due to out of memory
// the register tick function watches the memory utilization, and removes the data from the pages if it needs space.
// empty pages can always be recreated. The first place to go is memcache, the fallback is the database.

$GC_MaxMem = 270000000;
$GC_ClearTo = 250000000;
declare(ticks = 100)
	;
register_tick_function('GarbageCollection');

// DebugBreak();

// don't need this as of now, since we're not cleaning anything up anymore...

/**
 * Since this function uses global variable, therefore it will remain in this file
 */
function GarbageCollection(){
	global $GC_MaxMem, $GC_ClearTo, $DATASTORE;
	
	if(memory_get_usage() > $GC_MaxMem) // 170M
{
		// clsGlobalDataStores::CheckMemory($GC_ClearTo);
		// clsCachePages::CheckMemory($GC_ClearTo);
	}
}

// $CacheEnabled = Is_Kanan();

function getRandAlpha($length = 8){
	return getNewRandAlpha($length);
}

function getNewRandAlpha($length = 8){
	$code = md5(uniqid(rand(), true));
	return substr($code, 0, $length);
}

/**
 * This function uses string reverse, string position and string length functions
 *
 * @param mixed $string        	
 * @param mixed $item        	
 *
 */
function lastIndexOf($string, $item){
	$index = strpos(strrev($string), strrev($item));
	if($index) {
		$index = strlen($string) - strlen($item) - $index;
		return $index;
	}
	else
		return -1;
}

/**
 * This function is used to reutrn "ON" and "OFF"
 *
 * @param mixed $value        	
 * @return mixed
 *
 */
function OnOff($value){
	if($value === true) {
		return "On";
	}
	else {
		return "Off";
	}
}

/**
 * This function returns the <span></span> or Yes or No
 *
 * @param mixed $value        	
 * @param mixed $YesSpanCSS        	
 * @param mixed $NoSpanCSS        	
 * @return mixed
 *
 */

function YesNoText($value, $YesSpanCSS = '', $NoSpanCSS = ''){
	// DebugBreak();
	if(is_bool($value) || is_int($value)) {
		if($value === true) {
			if($YesSpanCSS) {
				return "<span style='$YesSpanCSS'>Yes</span>";
			}
			else {
				return "Yes";
			}
		}
		else {
			if($NoSpanCSS) {
				return "<span style='$NoSpanCSS'>No</span>";
			}
			else {
				return "No";
			}
		}
	}
	else {
		return $value;
	}
}

/**
 * This function return True or False
 *
 * @param mixed $value        	
 * @return mixed
 *
 */
function TrueFalse($value){
	// DebugBreak();
	if(is_bool($value) || is_int($value)) {
		if($value == true) {
			return 'True';
		}
		else {
			return 'False';
		}
	}
	else {
		return $value;
	}
}

function FixPath($path){
	$path = str_replace('\\', DIRECTORY_SEPARATOR, $path);
	$path = str_replace('/', DIRECTORY_SEPARATOR, $path);
	return $path;
}

function Redirect($DestLink = '', $JS = false, $ReturnInsteadOfPrint = false){
	global $pathtohttp, $ServerPort;
	$local_pathtohttp = $pathtohttp;
	$local_ServerPort = $ServerPort;
	
	if($local_ServerPort) {
		$arrPathExplode = explode("/", $local_pathtohttp);
		$arrPathExplode[2] .= ':' . $local_ServerPort;
		$local_pathtohttp = implode($arrPathExplode, '/');
	}
	
	if(!$JS) @ob_end_clean();
	
	if($DestLink === '') { // blank link means this page
		$link = $local_pathtohttp . substr($_SERVER['PHP_SELF'], 1);
	}
	elseif(strpos($DestLink, 'http') === false) {
		
		$cleanFileName = $DestLink;
		if(substr($DestLink, 0, 1) == '/' || substr($DestLink, 0, 1) == '\\') {
			$cleanFileName = substr($DestLink, 1);
		}
		$link = $local_pathtohttp . $cleanFileName;
	}
	else {
		$link = $DestLink;
	}
	
	if($JS) {
		$str = '<script type="text/javascript">';
		$str .= "<!-- \n";
		$str .= "window.location.href = '$link';\n";
		$str .= ' //-->';
		$str .= '</script>';
		
		if($ReturnInsteadOfPrint) {
			return $str;
		}
		else {
			print $str;
		}
	}
	else {
		header("Location: $link");
		exit();
	}
}

function AutoRefresh($DestLink = '', $Delay = 10, $JS = true){
	global $pathtohttp;
	if(!$JS) @ob_end_clean();
	
	if($DestLink === '') { // blank link means this page
		$link = $pathtohttp . substr($_SERVER['PHP_SELF'], 1);
	}
	else {
		$link = $DestLink;
	}
	if($JS) {
		$str = '<script type="text/javascript">';
		$str .= "<!-- \n";
		$str .= "setTimeout(function(){window.location.href = '$link';},$Delay*1000);\n";
		$str .= ' //-->';
		$str .= '</script>';
	}
	else {
		$str = "<meta http-equiv=\"refresh\" content=\"{$Delay};URL='{$link}'\" />";
	}
	print $str;
}

$arrUniqueCodes[] = '';

function getUniqueCode($length = 8){
	global $arrUniqueCodes;
	do {
		$code = md5(uniqid(rand(), true));
		$code = substr($code, 0, $length);
		// substr(str_shuffle(str_repeat('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789',$length)),0,$length);
		// check table to see if it's unique
	} while (in_array($code, $arrUniqueCodes));
	
	$arrUniqueCodes[] = $code;
	return $code;
}

function StringToMoney($strMoney){
	
	// because (intval((str_replace(',','',"2086.49")*100)/100) == 2086.48 for some really crazy reason...: http://php.net/manual/en/language.types.float.php
	$str = strval($strMoney); // convert to string
	$arr = str_split($str); // convert to char array
	
	$Dollars = 0;
	$CentLeft = $CentRight = 0;
	
	$workingOnCents = 0;
	
	foreach($arr as $number) {
		// verifiy it is a number
		if($number === ',') {
			// coma, do nothing
		}
		elseif($number === '.') {
			$workingOnCents = 1; // set to left cent
		}
		else {
			$int = intval($number);
			if($number != $int) {
				DebugBreak();
				die('Cannot convert ' . $strMoney . ' to money');
			}
			
			if($workingOnCents === 1) {
				$workingOnCents = 2;
				$CentLeft = $number;
			}
			elseif($workingOnCents === 2) {
				$CentRight = $number;
				return $Dollars . '.' . $CentLeft . $CentRight; // should always return from here
			}
			else { // dollars
				$Dollars *= 10;
				$Dollars += $number;
			}
		}
	}
	return $Dollars . '.' . $CentLeft . $CentRight; // should never return from here, but should still work
}

function IsLocal(){
	global $IsLocal;
	return $IsLocal;
}

// http://stackoverflow.com/questions/327097/simple-encryption-in-php
function ecrypt($str){
	return $str;
	$key = "CDH5VwN1q9DOnf0D4LIhj0aUyutyututyuti67867u45ertyhgh876r54redscxdECHFXmXHu0sW";
	
	// $Hash = substr(md5($str, true), 0, 4);
	
	for($i = 0; $i < strlen($str); $i++) {
		$char = substr($str, $i, 1);
		$keychar = substr($key, ($i % strlen($key)) - 1, 1);
		$char = chr(ord($char) + ord($keychar));
		$result .= $char;
	}
	
	$result = base64_encode($result);
	// remove the ==
	// substr($result,0,strlen($result)-2); // Not needed???
	$result = str_replace('=', '', $result); // Not needed???
	return $result;
}

function decrypt($str){
	return $str;
	$str = base64_decode($str . '==');
	$result = '';
	$key = "CDH5VwN1q9DOnf0D4LIhj0aUyutyututyuti67867u45ertyhgh876r54redscxdECHFXmXHu0sW";
	for($i = 0; $i < strlen($str); $i++) {
		$char = substr($str, $i, 1);
		$keychar = substr($key, ($i % strlen($key)) - 1, 1);
		$char = chr(ord($char) - ord($keychar));
		$result .= $char;
	}
	return $result;
}

function FormatPhoneNumber($PhoneToFormat){
	return preg_replace('~.*(\d{3})[^\d]*(\d{3})[^\d]*(\d{4}).*~', '($1) $2-$3', $PhoneToFormat);
}

function IsPhoneNumber($PhoneNumber){
	$sPattern = "/^
    (?:                                 # Area Code
    (?:                            
    \(                          # Open Parentheses
    (?=\d{3}\))                 # Lookahead.  Only if we have 3 digits and a closing parentheses
    )?
    (\d{3})                         # 3 Digit area code
    (?:
    (?<=\(\d{3})                # Closing Parentheses.  Lookbehind.
    \)                          # Only if we have an open parentheses and 3 digits
    )?
    [\s.\/-]?                       # Optional Space Delimeter
    )?
    (\d{3})                             # 3 Digits
    [\s\.\/-]?                          # Optional Space Delimeter
    (\d{4})\s?                          # 4 Digits and an Optional following Space
    (?:                                 # Extension
    (?:                             # Lets look for some variation of 'extension'
    (?:
    (?:e|x|ex|ext)\.?       # First, abbreviations, with an optional following period
    |
    extension               # Now just the whole word
    )
    \s?                         # Optionsal Following Space
    )
    (?=\d+)                         # This is the Lookahead.  Only accept that previous section IF it's followed by some digits.
    (\d+)                           # Now grab the actual digits (the lookahead doesn't grab them)
    )?                                  # The Extension is Optional
    $/x"; // /x modifier allows the expanded and commented regex
	
	if(preg_match($sPattern, $PhoneNumber, $aMatches)) {
		return true;
	}
	else {
		return false;
	}
}

function CleanPhone($PhoneNumber){
	return preg_replace("/[^0-9,.]/", "", $PhoneNumber);
}

function calculate_upc_check_digit($ean){
	$even = true;
	$esum = 0;
	$osum = 0;
	for($i = strlen($ean) - 1; $i >= 0; $i--) {
		if($even) $esum += $ean[$i];
		else $osum += $ean[$i];
		$even = !$even;
	}
	return (10 - ((3 * $esum + $osum) % 10)) % 10;
}

function lcase($String){
	return strtolower($String);
}

function ucase($String){
	return strtoupper($String);
}

function escape_html_specials($thestr){
	return (str_replace(array(
			'>',
			'<',
			'"',
			"'" 
	), array(
			'&gt;',
			'&lt;',
			'&quot;',
			'&#39;' 
	), $thestr));
}

function javascript_multiline_string($thestr){
	return (str_replace(array(
			"\n" 
	), array(
			'\n' 
	), $thestr));
}

function GetSize($Object){
	// xdebug_break();
	
	if(is_string($Object)) {
		return strlen($Object);
	}
	
	if(is_numeric($Object)) {
		return strlen($Object);
	}
	
	if(is_bool($Object)) {
		return 1;
	}
	
	if(is_null($Object)) {
		return 1;
	}
	
	if(is_object($Object)) {
		return strlen(serialize($Object));
	}
	
	if(!is_array($Object)) {
		return strlen(serialize($Object));
	}
	
	xdebug_break();
	// shouldn't be anything here
	return 1000;
}

function ReturnMemoryDetails(){
	xdebug_break();
	
	// allow 30 seconds to check memory
	set_time_limit(60);
	
	$CachePages = new clsCachePages();
	
	foreach(DataSets::$DataSets as $DSName => &$DataSet) {
		$CalculatedMemorySize = 0;
		if($DataSet->Pages) {
			foreach($DataSet->Pages as $PageNameStrings) {
				
				// this is where ALL the Page data is stored.
				clsCachePages::$CachePages[$DataSet->Name][$PageNameStrings['e']];
				
				$Page = $CachePages->GetPageObject($DataSet, $PageNameStrings['e']);
				
				$Page->CalculatedMemorySize = GetSize($Page);
				
				$CalculatedMemorySize += $Page->CalculatedMemorySize;
			}
		}
		$DataSet->CalculatedMemorySize = $CalculatedMemorySize;
	}
	
	$KeyField = DataSets::$DataSets[$DataSet]->IndexedColumn;
	
	if($PageRows) {
		foreach($PageRows as $row) {
			$row = current($row);
			foreach($row as $Name => $Data) {
				$e = 0;
			}
		}
	}
	
	set_time_limit(180); // allow 3 minutes from here

}

function IsValidEmail($Email){
	if(!filter_var($Email, FILTER_VALIDATE_EMAIL) === false) {
		// is a valid email, also add domain checking
		
		if(strpos($Email, 'noemail.com')) {
			return false;
		}
		
		return true;
	}
	else {
		return false;
	}
}

function S_Show($Number, $S = 's', $NotS = ''){
	if($Number == 1) return $NotS;
	else return $S;
}

function S_DontShow($Number, $S = 's', $NotS = ''){
	return S_Show($Number, $NotS, $S);
}

function ReadableDate($SqlDate, $AllowRoundedText = true){
	return Date_Readable($SqlDate, $AllowRoundedText);
}

function Date_Readable($SqlDate, $AllowRoundedText = true){
	if(is_numeric($SqlDate)) {
		$AsTime = $SqlDate;
	}
	else {
		$AsTime = strtotime($SqlDate);
	}
	
	if($AllowRoundedText) {
		$TimeDiff = abs($AsTime - realtime('NE'));
		
		if($TimeDiff < 50) $AsReadableDate = 'Just Now';
		elseif($TimeDiff < 5000) {
			$TimeDiff = $TimeDiff / 60; // now in minutes
			if(floor($TimeDiff) != 1) $s = 's';
			$AsReadableDate = floor($TimeDiff) . " minute$s ago";
		}
		else {
			$AllowRoundedText = false;
		
		}
	}
	
	if(!$AllowRoundedText) $AsReadableDate = date("F j, Y, g:i a", $AsTime);
	
	return $AsReadableDate;
}

function Date_ShortReadable($SqlDate, $AllowRoundedText = true){
	if(is_numeric($SqlDate)) {
		$AsTime = $SqlDate;
	}
	else {
		$AsTime = strtotime($SqlDate);
	}
	if($AllowRoundedText) {
		$TimeDiff = abs($AsTime - realtime('NE'));
		
		if($TimeDiff < 50) $AsReadableDate = 'Just Now';
		elseif($TimeDiff < 5000) {
			$TimeDiff = $TimeDiff / 60; // now in minutes
			if(floor($TimeDiff) != 1) $s = 's';
			else $s = '';
			$AsReadableDate = floor($TimeDiff) . " minute{$s} ago";
		}
		else {
			$AllowRoundedText = false;
		
		}
	}
	
	if(!$AllowRoundedText) $AsReadableDate = date("M j Y, g:ia", $AsTime);
	return $AsReadableDate;
}

function Date_ShortReadable_NoCurrentYear($SqlDate, $AllowRoundedText = true){
	if(is_numeric($SqlDate)) {
		$AsTime = $SqlDate;
	}
	else {
		$AsTime = strtotime($SqlDate);
	}
	if($AllowRoundedText) {
		$TimeDiff = abs($AsTime - realtime('NE'));
		
		if($TimeDiff < 50) $AsReadableDate = 'Just Now';
		elseif($TimeDiff < 5000) {
			$TimeDiff = $TimeDiff / 60; // now in minutes
			if(floor($TimeDiff) != 1) $s = 's';
			else $s = '';
			$AsReadableDate = floor($TimeDiff) . " minute{$s} ago";
		}
		else {
			$AllowRoundedText = false;
		
		}
	}
	
	if(!$AllowRoundedText) {
		if(date("Y", $AsTime) == date("Y")) {
			$AsReadableDate = date("j M g:ia", $AsTime);
		}
		else {
			$AsReadableDate = date("M j Y, g:ia", $AsTime);
		}
	}
	
	return $AsReadableDate;
}

function Date_ShortReadable_NoTime($SqlDate, $AllowRoundedText = true){
	if(is_numeric($SqlDate)) {
		$AsTime = $SqlDate;
	}
	else {
		$AsTime = strtotime($SqlDate);
	}
	if($AllowRoundedText) {
		$TimeDiff = abs($AsTime - realtime('NE'));
		
		if($TimeDiff < 50) $AsReadableDate = 'Just Now';
		elseif($TimeDiff < 5000) {
			$TimeDiff = $TimeDiff / 60; // now in minutes
			if(floor($TimeDiff) != 1) $s = 's';
			$AsReadableDate = floor($TimeDiff) . "m ago";
		}
		else {
			$AllowRoundedText = false;
		
		}
	}
	
	if(!$AllowRoundedText) $AsReadableDate = date("M j Y", $AsTime);
	return $AsReadableDate;
}

function Date_USCommon($SqlDate){
	if(is_numeric($SqlDate)) {
		$AsTime = $SqlDate;
	}
	else {
		$AsTime = strtotime($SqlDate);
	}
	
	$AsReadableDate = date("m/d/Y", $AsTime);
	return $AsReadableDate;
}

function build_csv($asoarr){
	$HeaderDone = false;
	foreach($asoarr as $arrRow) {
		foreach($arrRow as $name => &$field) {
			if(!$HeaderDone) {
				$arrHeader[] = str_replace('"', '""', $name);
			}
			$field = str_replace('"', '""', $field);
			$field = str_replace("\n", ' ', $field);
			$field = str_replace("\r", ' ', $field);
		}
		if(!$HeaderDone) {
			$string = '"' . implode('","', $arrHeader) . '"' . "\r\n";
			$HeaderDone = true;
		}
		$string .= '"' . implode('","', $arrRow) . '"' . "\r\n";
	}
	
	$size = strlen($string);
	$string = substr($string, 0, $size);
	return $string;
}

function Array_Merge_Safe($Array1, $Array2){
	if(is_array($Array1)) {
		$Good = true;
	}
	else {
		$Good = false;
	}
	
	if(is_array($Array2)) {
		if($Good) {
			// the one case where we continue....
		}
		else {
			return $Array2;
		}
	}
	else {
		if($Good) {
			return $Array1;
		}
		else {
			return null;
		}
	}
	
	return array_merge($Array1, $Array2);
}

function array_add($Array1, $Array2){
	if(is_array($Array1)) {
		$Good = true;
	}
	else {
		$Good = false;
	}
	
	if(is_array($Array2)) {
		if($Good) {
			// the one case where we continue....
		}
		else {
			return $Array2;
		}
	}
	else {
		if($Good) {
			return $Array1;
		}
		else {
			return null;
		}
	}
	
	// new code
	
	// $Insersect = array_intersect_key($Array1, $Array2);
	// $arrRet = array_merge($Array1, $Array2);
	$arrRet = $Array1;
	foreach($arrRet as $Key => &$Value) {
		if(isset($Array2[$Key])) {
			$Value += $Array2[$Key];
		}
	}
	
	foreach($Array2 as $Key2 => $Value2) {
		if(!isset($arrRet[$Key2])) {
			$arrRet[$Key2] = $Value2;
		}
	}
	
	// $Insersect = array_intersect_key($Array1, $Array2);
	// $arrRet = array_merge($Array1, $Array2);
	//
	// foreach($Insersect as $Key => $Value){
	// $arrRet[$Key] += $Value;
	// }
	//
	return $arrRet;
}

function array_subtract($Positive, $Negitive){
	if(is_array($Negitive)) {
		// reverse the array
		foreach($Negitive as $key => &$val) {
			$val = $val * -1;
		}
	}
	
	// then just add it
	
	return array_add($Positive, $Negitive);
}

/**
 *
 * See what values were removed from the Original array
 *
 * @param array $Original
 *        	The starting version of the array
 * @param array $New
 *        	The changed version of the array
 * @return array
 *
 *
 */
function array_removed_values($Original, $New){
	if(is_array($Original)) {
		if(!is_array($New)) {
			return $Original;
		}
		elseif($Original == $New) { // Both are arrays
			return array(); // both arrays are the same
		}
		else { // are array's but are different
			$arrRemoved = array();
			foreach($Original as $Value) {
				if(!in_array($Value, $New)) {
					$arrRemoved[] = $Value;
				}
			}
			return $arrRemoved;
		}
	}
	else {
		DebugBreak();
		return array();
	}
}

function GetTableColumnList($TableName, &$Prefix = ''){
	// DebugBreak();
	$res = sql_query('SHOW COLUMNS FROM ' . escape_string($TableName));
	$arrColumnName;
	while ($row = fetch_assoc($res)) {
		if(!isset($Prefix)) {
			$Prefix = str_split($row['Field']);
		}
		else {
			$PrefixTest = str_split($row['Field']);
			// $Prefix = array_intersect($Prefix, $PrefixTest); doesn't work, intersects all letters
			for($i = 0; $i < min(count($Prefix), count($PrefixTest)); $i++) {
				if($Prefix[$i] != $PrefixTest[$i]) {
					$Prefix = array_slice($Prefix, 0, $i);
					break;
				}
			}
		}
		$arrColumnName[$row['Field']] = $row['Field'];
	}
	if(count($Prefix) > 0) {
		// there was a prefix found, lets process the array
		foreach($arrColumnName as $Key => &$Value) {
			if(!is_object($Value)) {
				$Value = substr($Value, count($Prefix));
			}
		}
		// moved to here
		$Prefix = implode('', $Prefix);
	}
	// not sure what's up here, why would we implode something that might have a zero count
	// $Prefix = implode('', $Prefix);
	return $arrColumnName;
}


function AssocToUpdate($arrUpdates){
	if(isset($arrUpdates) && is_array($arrUpdates) && count($arrUpdates) > 0) {
		foreach($arrUpdates as $name => $value) {
			
			if(is_object($name)) {
				// do nothing
				LogError("Name of pair (" . get_class($name) . " / $value) in AssocToUpdate was a class.");
			}
			elseif(is_object($value)) {
				// do nothing
				DebugBreak();
				LogError("Value of pair ($name / " . get_class($value) . ") in AssocToUpdate was a class.");
			}
			elseif(is_numeric($value)) {
				$arr[] = "$name = $value";
			}
			else {
				$arr[] = "$name = '$value'";
			}
		}
		return implode(', ', $arr);
	}
	else {
		return false;
	}
}

function to4($Number, $showComma = true){
	if($showComma) {
		return number_format($Number, 4);
	}
	else {
		return number_format($Number, 4, '.', '');
	}
}

function to3($Number, $showComma = true){
	if($showComma) {
		return number_format($Number, 3);
	}
	else {
		return number_format($Number, 3, '.', '');
	}
}

function to2($Number, $showComma = true){
	if($showComma) {
		return number_format($Number, 2);
	}
	else {
		return number_format($Number, 2, '.', '');
	}
}

function to0($Number, $showComma = true){
	if($showComma) {
		return number_format($Number, 0);
	}
	else {
		return number_format($Number, 0, '.', '');
	}
}

function rd2($Number){
	return round($Number, 2);
}


function isWeekday($time){
	$date = date("w", $time);

	if($date != "0" && $date != "6") {
		return true;
	}
	else {
		return false;
	}
}


?>