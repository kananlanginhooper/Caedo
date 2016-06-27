<?php

// show all errors
$__ShowErrors = 'ALL';
error_reporting(E_ALL);

require_once "##CAEDO.inc";
// xdebug_break();

set_time_limit(60);

class index extends BasePage {

	public function __construct(){
		parent::__construct();
		$this->__JavaScript("/__LOCAL_USER_CLASSES/js/showdown.js", false, false);
	}
	
	// this __destruct function can be removed
	public function __destruct(){
		parent::__destruct();
	}
	
	// this __Run_Model function can be removed
	protected function __Run_Model(){
	
	}

	protected function __Route_To_View(){
	
	}

	protected function HEAD(){
		parent::HEAD();
		?>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

<?php
	}

	protected function addDoubleSpaces($str){
		// HTML has that nice way of ignoring any whitespace beyond one charactor, this is a way to get full breaks between sentances
		return str_replace('  ', '&nbsp; ', $str);
	}

	protected function BODY(){
		parent::BODY();
		
		// style='display: none'
		?>

<div class="row">
	<div class="col-lg-12">

		<pre id='README_md' style='display: none'>
<?php  print self::addDoubleSpaces(escape_html_specials(file_get_contents($this->__RootFolder . 'README.md'))); ?>
</pre>

<?php
		// set up the parse and display of the README.md file
		$this->__JavaScript("var converter = new showdown.Converter(); ", true, false);
		$this->__JavaScript("var html = converter.makeHtml($('#README_md').text().trim());", true, false);
		$this->__JavaScript("$('#Readme').html(html);", true, false);
		?>

<div id='Readme'></div>

	</div>
</div>

<?php
	}
}

$ThisPage = new index();




