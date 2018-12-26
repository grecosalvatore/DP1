<?php

if ((!isset($_SESSION['S241061_username']))||(!isset($_SESSION['S241061_password']))){
	Header( "HTTP/1.1 401 Unauthorized" ); 
	Header( "Location: login.php?msg=notAuthorized" );
	exit;
}



?>