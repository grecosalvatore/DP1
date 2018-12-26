<?php
	session_start();
	echo "logout";
	if((isset($_SESSION["S241061_username"])) && (isset($_SESSION["S241061_password"]))){
		
			 $_SESSION=array();
			 
			  if (ini_get("session.use_cookies")) {
        $params = session_get_cookie_params();
        setcookie(session_name(), '', time() - 3600*24,
            $params["path"], $params["domain"],
            $params["secure"], $params["httponly"]
        );
    }
    session_destroy();  // destroy session
	

	
    // redirect client to login page
    header('HTTP/1.1 307 temporary redirect');
	
	header('Location: index.php');
		
		
	}else{
		    // redirect client to login page
		header('HTTP/1.1 307 temporary redirect');
	
		header('Location: index.php');
		
	}

?>