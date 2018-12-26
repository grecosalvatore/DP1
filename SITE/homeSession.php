<?php 
//version of session for pages that not need login
	session_start(); 

if (!isset($_SESSION['S241061_time'])){
		$_SESSION['S241061_time'] = time();
}

$t=time();
$diff=0;
$new=false;

if (isset($_SESSION['S241061_time'])){
	
    $t0=$_SESSION['S241061_time'];
    $diff=($t-$t0);  // inactivity period
} else {
    $new=true;
}
if ($new || ($diff > 120)) { // new or with inactivity period too long
    
    $_SESSION=array();

    // If it's desired to kill the session, also delete the session cookie.
    // Note: This will destroy the session, and not just the session data!
    if (ini_get("session.use_cookies")) {
        $params = session_get_cookie_params();
        setcookie(session_name(), '', time() - 3600*24,
            $params["path"], $params["domain"],
            $params["secure"], $params["httponly"]
        );
    }
    session_destroy();  // destroy session
	
} else {
    $_SESSION['S241061_time']=time(); /* update time */
	
}
?> 
