<?php
	// return true if the cookie are enabled

	
ob_start();
setcookie("cookietest", "cookie_value", time()+3600);	
if (!isset($_COOKIE["cookietest"])){
	if(empty($_GET['status'])){
		header("Location:".$_SERVER['PHP_SELF']."?status=1");
		exit;
	}
	ob_flush();
}		
		
	function get_cookie_enabled(){
		if (isset($_COOKIE["cookietest"])&&($_COOKIE["cookietest"] == "cookie_value")){
			return true;
		}else{
			return false;				
		}
    
	}
	

?>