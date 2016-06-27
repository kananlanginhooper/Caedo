<?php

// I'm listing BasePage first in this file, because it will be the most often looked at class in here, you may have to step through parts of it to render you page, having it at the top makes it easy to find
class BasePage {
	
	// Global Varibles
	protected $__RootFolder;
	protected $__CaedoRootFolder;
	
	// Setup
	protected $__PageType = 'HTML5'; // Options HTML5, HTML, JSON, RAW -- These are validated in __destruct
	                                 
	// Before Body Tag
	protected $__arrHTMLHead = array();
	protected $__PageTitle = '';
	protected $__arrCSS_Early = array();
	
	// - Early Javascript
	protected $__HasJavaScript_Early_Been_Processed = false;
	protected $__arrJavaScript_Early = array();
	protected $__arrJavaScript_Early_Source = array();
	protected $__arrJavaScript_Early_files = array();
	protected $__arrJavaScript_Early_Ready_For_Output = array();
	
	// Body
	protected $__arrBody = array();
	protected $__arrBodyEnd = array();
	
	// After Body Tag
	// - Early Javascript
	protected $__HasJavaScript_Late_Been_Processed = false;
	protected $__arrJavaScript_Late = array();
	protected $__arrJavaScript_Late_Source = array();
	protected $__arrJavaScript_Late_files = array();
	protected $__arrJavaScript_Late_Ready_For_Output = array();
	
	// Tidy Comment
	protected $__TidyHTMLComment = '<!-- Framework will Insert Tidy Message Here - This tag will be replaced with the Tidy HTML messages and Stats on Memory usage and Run time - DO NOT EDIT THIS COMMENT -->';
	
	// Other varables
	private $__ViewData;
	private $__IsPrinter = FALSE;
	private $__OutputBuffer;
	private $__OB_started = false;
	private $__ConstructorCalled = false;

	public function __construct(){
		
		__Tag('Starting page class ' . get_class($this));
		//
		
		$this->__ConstructorCalled = true;
		
		$this->__AddGeneralFunctionSets();
		
		// we're autoloading jquery, and making it a dependency of the framework. I think it will be used in most
		$this->__JavaScript("//code.jquery.com/jquery-2.1.3.min.js", false, true);
		
		// Save Site globals from Caedo.inc
		$this->__RootFolder = Site::$RootFolder;
		$this->__CaedoRootFolder = Site::$CaedoRootFolder;
	}

	protected function __Verify_ConstructorCalled(){
		// check to make sure constuctor was called
		if(!$this->__ConstructorCalled) {
			die("The BasePage constructor was not called");
		}
	}

	protected function __Verify_Classname_Matches_Filename(){
		$StartingPageFileName = $_SERVER['PHP_SELF'];
		$StartingPageFileName = substr($StartingPageFileName, 0, strlen($StartingPageFileName) - 4); // to remove the .php
		
		$arrFilePath = explode('/', $StartingPageFileName); // split into folders
		$arrFilePath = array_reverse($arrFilePath);
		$ImpliedClassName = $arrFilePath[0];
		
		// 404.php gets a pass, as this our default not-found page
		if(lcase(get_class($this)) !== lcase($ImpliedClassName) && $ImpliedClassName != '404') {
			xdebug_break();
			die("Classname doesn't match file name");
		}
	}

	protected function __AddGeneralFunctionSets(){
		global $RootFolder;
		
		// for now I moved this to global
		// require_once $RootFolder . '__local/data_set_definitions.inc';
	}

	private function __CSS(){
		if(isset($this->__arrCSS_Early) && is_array($this->__arrCSS_Early)) {
			foreach($this->__arrCSS_Early as $CSSLine) {
				print '  ' . $CSSLine . "\n";
			}
		}
	}

	private function __Process_CSS(){
		// for now, I'm not building out CSS processing. It'll happen, but more worried about JS at this point
	}

	protected function __JavaScript($SourceOrLink, $IsSource = false, $IsEarly = false){
		// $SourceOrLink = trim($SourceOrLink);
		
		if($IsEarly && $IsSource) {
			if($this->__HasJavaScript_Early_Been_Processed) die('Error: JavaScript Early Has Already Been Processed');
			$this->__arrJavaScript_Early_Source[] = $SourceOrLink;
		}
		elseif($IsEarly && !$IsSource) {
			if($this->__HasJavaScript_Early_Been_Processed) die('Error: JavaScript Early Has Already Been Processed');
			$this->__arrJavaScript_Early_files[] = $SourceOrLink;
		}
		elseif(!$IsEarly && $IsSource) {
			if($this->__HasJavaScript_Late_Been_Processed) die('Error: JavaScript Late Has Already Been Processed');
			$this->__arrJavaScript_Late_Source[] = $SourceOrLink;
		}
		elseif(!$IsEarly && !$IsSource) {
			if($this->__HasJavaScript_Late_Been_Processed) die('Error: JavaScript Late Has Already Been Processed');
			$this->__arrJavaScript_Late_files[] = $SourceOrLink;
		}
	
	}

	private function __Process_EarlyJavaScript(){
		// the point of this function is to turn the JS arrays into exportable lines of html
		
		$this->__HasJavaScript_Early_Been_Processed = true;
		// to be backwards compatible
		if(count($this->__arrJavaScript_Early)) {
			foreach($this->__arrJavaScript_Early as $JavaScriptLine) {
				$this->__arrJavaScript_Early_Ready_For_Output[] = $JavaScriptLine;
			}
		}
		
		// files
		if(count($this->__arrJavaScript_Early_files)) {
			foreach($this->__arrJavaScript_Early_files as $JavaScriptLine) {
				$this->__arrJavaScript_Early_Ready_For_Output[] = "<script src='" . $JavaScriptLine . "'></script>";
			}
		}
		
		// source
		if(count($this->__arrJavaScript_Early_Source)) {
			$str = "  <script> \n";
			$str .= "   $( document ).ready(function() { \n";
			foreach($this->__arrJavaScript_Early_Source as $JavaScriptLine) {
				$str .= '    ' . $JavaScriptLine . "\n";
			}
			$str .= "   });\n";
			$str .= "  </script>\n";
			
			$this->__arrJavaScript_Early_Ready_For_Output[] = $str;
		}
	}

	private function __EarlyJavaScript(){
		if(count($this->__arrJavaScript_Early_Ready_For_Output)) {
			foreach($this->__arrJavaScript_Early_Ready_For_Output as $JavaScriptLine) {
				print "$JavaScriptLine\n";
			}
		}
	}

	private function __Process_LateJavaScript(){
		// the point of this function is to turn the JS arrays into exportable lines of html
		
		$this->__HasJavaScript_Late_Been_Processed = true;
		// to be backwards compatible
		if(count($this->__arrJavaScript_Late)) {
			foreach($this->__arrJavaScript_Late as $JavaScriptLine) {
				$this->__arrJavaScript_Late_Ready_For_Output[] = $JavaScriptLine;
			}
		}
		
		// files
		if(count($this->__arrJavaScript_Late_files)) {
			foreach($this->__arrJavaScript_Late_files as $JavaScriptLine) {
				$this->__arrJavaScript_Late_Ready_For_Output[] = "<script src='" . $JavaScriptLine . "'></script>";
			}
		}
		
		// source
		if(count($this->__arrJavaScript_Late_Source)) {
			$str = "  <script> \n";
			$str .= "   $( document ).ready(function() { \n";
			foreach($this->__arrJavaScript_Late_Source as $JavaScriptLine) {
				$str .= '    ' . $JavaScriptLine . "\n";
			}
			$str .= "   });\n";
			$str .= "  </script>\n";
			
			$this->__arrJavaScript_Late_Ready_For_Output[] = $str;
		}
	}

	private function __LateJavaScript(){
		if(count($this->__arrJavaScript_Late_Ready_For_Output)) {
			foreach($this->__arrJavaScript_Late_Ready_For_Output as $JavaScriptLine) {
				print "$JavaScriptLine\n";
			}
		}
	}
	
	// This is where the output comes from. After all other code has run, this will triger and output the page.
	
	// NOTICE:
	// if you call end();, this will not output anything.
	// if this page shows blank for any other reason you can manually call this destructor without any ill effect
	// if you override the destruct method in the child class, call this distruct after all the child code is run
	
	public function __destruct(){
		global $IsLocal;
		
		$this->__Verify_ConstructorCalled();
		
		$this->__Verify_Classname_Matches_Filename();
		
		// this is where all the magic happens, we want to delay these calls for as long as posible and make sure they're literally the last functions called.
		// That gives the page class all the time in the world to change the header and body-view based on the model and routing
		
		// xdebug_break();
		
		// Run all PHP code first. Then run Javascript and CSS. Even if it's bad form to add javascript in the view, we're going to support it.
		
		// -- PHP CODE --
		
		// validate page type
		$this->__PageType = ucase($this->__PageType);
		switch ($this->__PageType) {
			case 'HTML5' :
			case 'HTML' :
			case 'JSON' :
			case 'RAW' :
				// OK
				break;
			default :
				die('Unsupported __PageType');
		}
		
		if($this->__PageType == 'JSON' || $this->__PageType == 'RAW') {
			// you use these for non-human (non-browser) consumption. Here are some use cases:
			// Ajax calls, authentication, form posting in some cases, API callback pages, webservices in some cases
			// Downloading files, such as CSV, excel and PDF creation.
			
			// since these are non-HTML, we are not going to call the HEAD funciton, no javascript loading, no CSS.
			// We are also not going to call the Footer. __construct and BODY is the only page class functions called.
			
			// Tidy is also not run, in any cases, and errors are not displayed, even in test environments
			
			// Body Section
			$this->__PrintCapture_Start();
			
			// Get MainView function
			$MainView = $this->MainView;
			$this->$MainView();
			
			// footer is not called
			$PageBODY = $this->__PrintCapture_EndReturn();
			
			// nothing else is output, BODY function only
			$PageData = $PageBODY;
			
			// OUTPUT
			echo $PageData; // This is the main print of all of the HTML code, if not using tidy
		
		}
		else {
			
			// Head Section
			$this->__PrintCapture_Start();
			$this->HEAD(); // call the HEAD from the page class
			$PageHEAD = $this->__PrintCapture_EndReturn();
			
			// Body Section
			$this->__PrintCapture_Start();
			
			// Get MainView function
			$MainView = $this->MainView;
			$this->$MainView();
			
			$this->FOOTER();
			$PageBODY = $this->__PrintCapture_EndReturn();
			
			// -- HTML, JS and CSS --
			
			$this->__Process_CSS();
			$this->__Process_EarlyJavaScript();
			$this->__Process_LateJavaScript();
			
			// head section
			$this->__PrintCapture_Start();
			$this->__DOCTYPE();
			$this->__HTML();
			
			$this->__HEAD_open();
			$this->__CSS();
			print $PageHEAD; // yeah, I'm reusing this varible.
			$this->__EarlyJavaScript();
			$this->__HEAD_close();
			
			$PageHEAD = $this->__PrintCapture_EndReturn();
			
			// Body Section -- Already has run above
			
			// export section, sort through the code that was exported from the page class, clean and send to the screen
			
			// Closing JS, Body and html
			$this->__PrintCapture_Start();
			
			$this->__LateJavaScript();
			$this->__Closing_BODY_HTML();
			
			$PageEND = $this->__PrintCapture_EndReturn();
			
			// collect the three sections
			$PageHTML = $PageHEAD . $PageBODY . $PageEND;
			
			if(isset(Site::$TidyUp) && Site::$TidyUp) {
				
				// check that Tidy extention is loaded
				if(extension_loaded('tidy')) {
					
					// Clean up the HTML
					$tidy = new tidy();
					$tidy_config = array(
							'indent' => true,
							'output-xhtml' => true,
							'wrap' => 400,
							'new-blocklevel-tags' => 'audio, source' 
					);
					
					$tidy->parseString($PageHTML, $tidy_config, 'utf8');
					$tidy->cleanRepair();
					
					// Get the Tidy Message, then change the page source that is now nicely formatted, and add the Tidy Message
					$PageHTMLAfterTidy = $tidy->value;
				}
				else {
					// tidy not enabled
					$PageHTMLAfterTidy = $PageHTML;
				}
				
				// check and see if there is a Replacement tag. If the tag is edited here or in a page class, it will be an issue.
				if(strpos($PageHTMLAfterTidy, $this->__TidyHTMLComment)) {
					
					$TidyMessage = $this->__ReturnHTMLErrorsForLocalhost($PageHTMLAfterTidy);
					
					$PageHTML = str_replace($this->__TidyHTMLComment, $TidyMessage, $PageHTMLAfterTidy);
				}
				else {
					$PageHTML = $PageHTMLAfterTidy;
				}
				
				if($this->__PageType == 'HTML5') {
					// remove doctype if there is one
					$PageHTML = preg_replace("/<!DOCTYPE((.|\n|\r)*?)\">/", "", $PageHTML);
					
					// also remove HTML crap it can sometimes add
					$PageHTML = preg_replace("/<html((.|\n|\r)*?)\">/", "", $PageHTML);
					
					// OUTPUT
					$this->__DOCTYPE(); // DOCTYPE is removed or messed up by tidy
				}
				
				echo "\n<html>\n";
				echo $PageHTML; // This is the main print of all of the HTML code
			}
			else {
				// OUTPUT
				echo $PageHTML; // This is the main print of all of the HTML code, if not using tidy
			}
		}
		
		// Cleanup and Localhost functions
		
		if(ob_get_level() > 1) {
			xdebug_break();
			die('You still have an output buffer open.');
			
			// clean until there is nothing left to clean
			while (ob_get_level()) {
				ob_end_clean();
			}
		}
		
		__Tag('Ending page class ' . get_class($this));
	
	}

	protected function __DOCTYPE(){
		print "<!DOCTYPE html>\n";
	}

	protected function __HTML(){
		print "<html>\n";
	}

	protected function __HEAD_close(){
		print " </head>\n";
	}

	protected function __HEAD_open(){
		
		print " <head>\n";
		print "  <title>{$this->__PageTitle}</title>\n";
		
		if(isset($this->__arrHTMLHead) && is_array($this->__arrHTMLHead)) {
			foreach($this->__arrHTMLHead as $HeadLine) {
				print '  ' . $HeadLine . "\n";
			}
		}
	
	}

	protected function HEAD(){
		print '<meta name="viewport" content="width=device-width, initial-scale=1.0">';
	}
	
	//
	//
	//
	
	private $MainView = 'BODY';

	protected function __SelectAlternateView($AlternateView){
		// use this function to call a function other than BODY() for the main display
		$this->MainView = "BODY__{$AlternateView}";
	}
	
	protected $__arrViewData = array();

	protected function __SetViewData($ViewLocalVaribleName, $Data){
		// use this function to set data that will be available to the view as a local varible
		$this->__arrViewData[$ViewLocalVaribleName] = $Data;
	}

	protected function BODY(){
	
	}
	
	//
	//
	//
	
	protected function FOOTER(){
	
	}

	protected function __BODY(){
		print " <body>\n";
		if(isset($this->__arrBody) && is_array($this->__arrBody)) {
			foreach($this->__arrBody as $BodyLine) {
				print '  ' . $BodyLine . "\n";
			}
		}
		
		if(isset($this->__arrBodyEnd) && is_array($this->__arrBodyEnd)) {
			foreach($this->__arrBodyEnd as $BodyLine) {
				print '  ' . $BodyLine . "\n";
			}
		}
	
	}

	protected function __Closing_BODY_HTML(){
		print " </body>\n";
		print '</html>';
	}

	private function __ReturnHTMLErrorsForLocalhost($html){
		// put this in a div, there should only ever be one
		// display: block !important; is so you can't hide this using css.
		// If you are reading this and need to hide this message, you need to change Site::$ShowErrors, this is usually
		
		$ret = "";
		
		$ShowStats = (isset(Site::$ShowStats) && Site::$ShowStats);
		$ShowErrors = (isset(Site::$ShowErrors) && Site::$ShowErrors);
		
		if($ShowStats || $ShowErrors) {
			$ret .= "\n\n<br />\n<br />\n<br />\n<br />\n<br />\n<br />\n<br />\n";
		}
		
		if($ShowStats) {
			$ret .= "<hr />";
			$ret .= __ReturnTags();
		}
		
		if($ShowErrors) {
			if(extension_loaded('tidy')) {
				$tidy = new tidy();
				$tidy_config = array(
						'indent' => true,
						'output-xhtml' => true,
						'wrap' => 400 
				);
				$tidy->parseString($html, $tidy_config, 'utf8');
				$tidy->cleanRepair();
				
				$ErrorsToDisplay = array();
				// strlen($tidy->errorBuffer)
				
				$ErrorBuffer = $tidy->errorBuffer;
				$arrError = explode("\n", $ErrorBuffer);
				
				foreach($arrError as $Error) {
					if(strpos($Error, 'Warning: <table> lacks "summary" attribute')) {
						// don't export
					}
					else {
						$ErrorsToDisplay[] = htmlspecialchars($Error) . "\n<br />";
					}
				}
				
				// xdebug_break();
				if(count($ErrorsToDisplay)) {
					$ret .= "\n\n<br />\n<br />\n";
					$ret .= "<hr>";
					$ret .= "There are errors on this page:<br />\n";
					$ret .= implode($ErrorsToDisplay);
				}
			}
			else {
				$ret .= "\n\n<br />\n<br />\n";
				$ret .= 'Tidy was enabled, but not installed.  Please check you php.ini and phpinfo() function to see if the tidy extension is loaded.';
			}
		}
		
		return $ret;
	}
	
	// -------------- Print Capture Tool
	
	protected function __PrintCapture_Start(){
		ob_start();
	}

	protected function __PrintCapture_EndReturn(){
		$JustCaptured_OutputBuffer = ob_get_clean();
		
		// return what we just captured
		return $JustCaptured_OutputBuffer;
	}
	
	// -------------- Print Capture Tool
	
	// --------------- Utility Functions
	protected function __HeaderRedirect($Page, $Domain = ''){
		header('Location: ' . $Domain . $Page);
		exit();
	}
	
	// --------------- Utility Functions

}

//
//
//
//
//
class Site {
	static $xdebug_toggle = false;
	static $clsSiteConfig;
	
	// Core folders
	static $RootFolder, $CaedoRootFolder, $CaedoConfigFolder;
	
	// users list
	static $Users;
	static $UserPhoneNumber; // this should really get moved to users
	                         
	// Add in the globals for the optional modules?
	static $CDN, $scriptAddress, $CDNDataAddress, $CDNDataBucket;
	
	// from SiteConfig
	static $IsLocal, $IsSecure, $Http_Host, $Http_Port, $PathToHttp, $pathToSsl;
	
	static $Url;
	static $Site;
	static $Env;
	
	// URL (Site) overriding
	static $AllowURLOverride, $URLOverridePassKey;
	
	// session
	static $SessionManager, $SessionManagerPath;
	
	// Caedo flags
	static $TidyUp, $ShowErrors, $ShowStats;
	
	// Database
	static $ShouldHaveDatabase, $HasDatabase, $WriteDatabase, $HasReadReplica, $ReadDatabase;

	static function setSite(clsSiteConfig $clsSiteConfig){
		// save the class, don't really need it, but we'll save it anyway for now
		Site::$clsSiteConfig = $clsSiteConfig;
		
		Site::$Url = $clsSiteConfig->Url;
		Site::$Site = $clsSiteConfig->Site;
		Site::$Env = $clsSiteConfig->Env;
		Site::$IsLocal = $clsSiteConfig->IsLocal;
		Site::$WriteDatabase = $clsSiteConfig->WriteDatabase;
		Site::$HasReadReplica = $clsSiteConfig->HasReadReplica;
		Site::$ReadDatabase = $clsSiteConfig->ReadDatabase;
		Site::$AllowURLOverride = $clsSiteConfig->AllowURLOverride;
		Site::$URLOverridePassKey = $clsSiteConfig->URLOverridePassKey;
		Site::$SessionManager = $clsSiteConfig->SessionManager;
		Site::$SessionManagerPath = $clsSiteConfig->SessionManagerPath;
		Site::$TidyUp = $clsSiteConfig->TidyUp;
		Site::$ShowErrors = $clsSiteConfig->ShowErrors;
		Site::$ShowStats = $clsSiteConfig->ShowStats;
		Site::$IsSecure = $clsSiteConfig->IsSecure;
		Site::$Http_Host = $clsSiteConfig->Http_Host;
		Site::$Http_Port = $clsSiteConfig->Http_Port;
		Site::$PathToHttp = $clsSiteConfig->PathToHttp;
		Site::$pathToSsl = $clsSiteConfig->pathToSsl;
	
	}
}

class clsDatabaseConnections {
	private static $arrConnections = array();

	public static function AddConnection($Env, $UserName, $Password, $HostURL, $ReadOnly, $DateVersioned){
		if(!$ReadOnly) {
			self::$arrConnections[$Env][0] = new clsDatabaseConnection($UserName, $Password, $HostURL, false, $DateVersioned);
		}
		else {
			self::$arrConnections[$Env][] = new clsDatabaseConnection($UserName, $Password, $HostURL, true, $DateVersioned);
		}
	}

	public static function GetConnection($Env, $ReadOnly){
		if(!$ReadOnly) {
			if(isset(self::$arrConnections[$Env][0])) {
				return self::$arrConnections[$Env][0];
			}
			else {
				return false;
			}
		}
		else {
			$Readonlys = count(self::$arrConnections[$Env]) - 1;
			$cn = rand(1, $Readonlys);
			if(isset(self::$arrConnections[$Env][$cn])) {
				return self::$arrConnections[$Env][$cn];
			}
		}
	}

	public static function ListConnections(){
		if(count(self::$arrConnections)) {
			return array_keys(self::$arrConnections);
		}
	}

	public static function SecurityCleanUp(){
		self::$arrConnections = null;
	}

}

class clsDatabaseConnection {
	public $UserName, $Password, $HostURL, $ReadOnly, $DateVersioned;

	public function __construct($UserName, $Password, $HostURL, $ReadOnly, $DateVersioned){
		$this->UserName = $UserName;
		$this->Password = $Password;
		$this->HostURL = $HostURL;
		$this->ReadOnly = $ReadOnly;
		$this->DateVersioned = $DateVersioned;
	}
}

class clsSiteConfigs {
	public static $DefaultSite = false;
	private static $arrSiteConfigs = array();

	public static function AddSiteConfig(clsSiteConfig $clsSiteConfig){
		self::$arrSiteConfigs[$clsSiteConfig->Url] = $clsSiteConfig;
	}

	public static function GetSiteConfig($OverrideHttp_Host = ''){
		
		$arrHttp_Host = clsSiteConfig::ParseHttp_Host($OverrideHttp_Host);
		
		$Url = $arrHttp_Host[0];
		if(isset(self::$arrSiteConfigs[$Url])) {
			return self::$arrSiteConfigs[$Url];
		}
		else {
			if(self::$DefaultSite && isset(self::$arrSiteConfigs[self::$DefaultSite])) {
				return self::$arrSiteConfigs[self::$DefaultSite];
			}
			else {
				return false;
			}
		}
	}

	public static function ListSiteConfig(){
		if(count(self::$arrSiteConfigs)) {
			return array_keys(self::$arrSiteConfigs);
		}
	}

	public static function SecurityCleanUp(){
		self::$arrSiteConfigs = null;
	}
}

class clsSiteConfig {
	public $Url, $Site, $Env, $IsLocal, $WriteDatabase, $HasReadReplica, $ReadDatabase, $AllowURLOverride, $URLOverridePassKey, $SessionManager, $SessionManagerPath, $TidyUp, $ShowErrors, $ShowStats;
	public $IsSecure, $Http_Host, $Http_Port, $PathToHttp, $pathToSsl;

	public function __construct($Url, $Site, $Env, $IsLocal, $WriteDatabase, $HasReadReplica, $ReadDatabase, $AllowURLOverride, $URLOverridePassKey, $SessionManager, $SessionManagerPath, $TidyUp, $ShowErrors, $ShowStats){
		$this->Url = strtolower($Url);
		$this->Site = $Site;
		$this->Env = $Env;
		$this->IsLocal = $IsLocal; // if you are working of localhost, becareful, this can get set to be true, even if try and set it to false
		                           
		// This will change page by page, true on secure pages and false on insure ones
		                           // Fake secure on localhost, otherwise some pages won't load. Devs and testers need to be smart enough not to enter real credit card numbers into a test site
		if($this->IsLocal) {
			$this->IsSecure = true;
		}
		else {
			$this->IsSecure = ((isset($_SERVER['HTTPS']) && $_SERVER['HTTPS']) || (isset($_SERVER["HTTP_X_FORWARDED_PROTO"]) && $_SERVER["HTTP_X_FORWARDED_PROTO"] == 'https'));
		}
		
		// Port based vhosts are my personal preference
		// apache has the unfortunate default of including the port number in the hostname
		// I split them apart here
		$arrHttp_Host = self::ParseHttp_Host();
		$this->Http_Host = $arrHttp_Host[0];
		$this->Http_Port = $arrHttp_Host[1];
		
		// this helps with redirection from secure and back.
		// It is the recomendation to serve all pages over https, there are cases where you can't
		// unsecured external resources such as javascript and iframes being one of the major ones.
		
		$this->PathToHttp = 'http://' . strtolower($_SERVER["HTTP_HOST"]) . '/';
		
		if($this->IsLocal) {
			// At this time we are not offering SSL on localhost.
			// this is a place I'm sure we will want to improve on in the future
			// SSL can be done on localhost, I've just personaly never gotten it totally setup
			// At some point, someone is going to want this.
			// Feel free to expand the code to support localhost SSL.
			$this->pathToSsl = $this->PathToHttp;
		}
		else {
			// Again, this is not for localhost, if it was and you were using port based vhosts
			// you would need to change this so you were specifing an http port and an https port
			// as it stands, we're going to force it to use the browser default of 443
			// Again, at some point, someone is going to want this.
			// Feel free to expand the code to support specified port SSL.
			$this->pathToSsl = 'https://' . strtolower($_SERVER["HTTP_HOST"]) . '/';
		}
		
		$this->WriteDatabase = $WriteDatabase;
		$this->HasReadReplica = $HasReadReplica;
		
		$this->ReadDatabase = $ReadDatabase;
		$this->AllowURLOverride = $AllowURLOverride;
		$this->URLOverridePassKey = $URLOverridePassKey;
		
		$this->SessionManager = $SessionManager;
		$this->SessionManagerPath = $SessionManagerPath;
		
		$this->TidyUp = $TidyUp;
		$this->ShowErrors = $ShowErrors;
		$this->ShowStats = $ShowStats;
	}

	static function ParseHttp_Host($HostToParse = ''){
		if(!$HostToParse) {
			$HostToParse = strtolower($_SERVER['HTTP_HOST']);
		}
		else {
			$HostToParse = strtolower($HostToParse);
		}
		
		if(strpos($HostToParse, ':')) {
			$arrPath = explode(':', $HostToParse);
			if(count($arrPath) != 2) {
				xdebug_break();
				die('bad HTTP_HOST in clsSiteConfig::ParseHttp_Host.  Host should have either zero or one colons');
			}
			// this is a string, we're going to use it as a string way more often than an int
			// if the future I could see
			$arrPath[1] = ':' . $arrPath[1];
		}
		else {
			// no port means port 80 or 443, but browsers are smart enough to fill it in, so unless it's an override, leave it blank
			$arrPath[0] = $_SERVER['HTTP_HOST'];
			$arrPath[1] = '';
		}
		return $arrPath;
	}
}

class DB {
	public static $dbh; // Is Connected
	public static $mysqli;
	public static $mysqli_write;
	public static $connectionDetails_write;
	public static $mysqli_read;
	public static $connectionDetails_read;
	public static $mysqli_base;
	public static $connectionDetails_base;
}

class Instance {
	public static $PublicDNS;
	public static $InstanceID;
	public static $InstanceType;
	public static $LoggingEnabled = false;
	public static $SQLDebug_MemcacheStarted = false;

	public function __construct(){
		// DebugBreak();
		self::$LoggingEnabled = (self::$SQLDebug_MemcacheStarted && time() < (int)cacheGet('SQLDebug') && (cacheGet('SQLDebug_Session') == 'All' || cacheGet('SQLDebug_Session') == session_id()));
	}

	public static function FindInstanceDetails(){
		self::$PublicDNS = shell_exec("/usr/bin/curl -s http://169.254.169.254/latest/meta-data/public-hostname");
		self::$PublicDNS = shell_exec("/usr/bin/curl -s http://169.254.169.254/latest/meta-data/public-ipv4");
		self::$InstanceID = shell_exec("/usr/bin/curl -s http://169.254.169.254/latest/meta-data/instance-id");
		self::$InstanceType = shell_exec("/usr/bin/curl -s http://169.254.169.254/latest/meta-data/instance-type");
	}

	public static function LoadInstanceDetails($Packed){
		if(!$Packed) return false;
		$arr = explode('||', $Packed);
		self::$PublicDNS = $arr[0];
		self::$InstanceID = $arr[1];
		self::$InstanceType = $arr[2];
	}

	public static function ExportInstanceDetails(){
		return self::$PublicDNS . '||' . self::$InstanceID;
		'||' . self::$InstanceType;
	}
}



