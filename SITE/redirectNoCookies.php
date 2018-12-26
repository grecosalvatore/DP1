<?php


	
	//if cookie are not enabled the navigation is forbidden
	if (get_cookie_enabled()==false){
		header('HTTP/1.1 307 temporary redirect');
		header('Location: index.php');
		exit();
	}
	
	
	
?>