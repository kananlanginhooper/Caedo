<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<title>Technical Difficulties</title>
<style type="text/css">
/* <![CDATA[ */
body {
	
}
/* ]]> */
</style>
</head>
<body>
            <?php
												if(isset($_SESSION['SiteProfile']['image'])) {
													print '<img style="padding-top: 50px; text-align: center; margin-left: auto; margin-right: auto; display: block;" src="' . $_SESSION['SiteProfile']['image'] . '" alt="Logo" />';
												}
												?>

            <h1 style="text-align: center;">We are experiencing technical difficulties.</h1>

	<h1 style="text-align: center;">Please check back soon!</h1>

	<!-- <?php echo  $Error ?> -->
</body>
</html>