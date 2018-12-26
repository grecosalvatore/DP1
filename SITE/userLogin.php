<?php 
	
// sanitize input string
function sanitizeString($conn,$var) {
	$var = strip_tags($var);
	$var = htmlentities($var);
	$var = stripcslashes($var);
	$var = mysqli_real_escape_string($conn,$var);
	return $var;
}
		
		
		session_start();
	
		
		if ((isset($_POST["logUserEmail"])) && (isset($_POST["logUserPsw"]))){
				
			
		
			include 'connectionDb.php';
			
	
			mysqli_select_db($conn,'users');
	
			//input validation
			$email = sanitizeString($conn,$_POST["logUserEmail"]);
			$psw = sanitizeString($conn,$_POST["logUserPsw"]);
			
			

			//hash psw with md5
			$psw = md5($psw);
		
			
		
			$query = "SELECT count(*) FROM users WHERE email ='".$email."' and password= '".$psw."'"; 
			$res = @mysqli_query($conn, $query); 
			$row = mysqli_fetch_array($res, MYSQLI_NUM);
	
			$count = $row[0];
			
		
			if ( $count > 0){
				echo"succes login";
				//save username and psw in the session
				//$_SESSION["S241061_username"]=$_POST["logUserEmail"]; 
				//$_SESSION["S241061_password"]=$_POST["logUserPsw"]; 
				$_SESSION["S241061_username"]=$email; 
				$_SESSION["S241061_password"]=$psw;
				$_SESSION['S241061_time']=time();
			
				echo $_SESSION['S241061_time'];
				
				Header( "HTTP/1.1 307 Temporary Redirect" ); 
				Header( "Location: personalPage.php" );
			}else{
				
				Header( "HTTP/1.1 307 Temporary Redirect" ); 
				Header( "Location: login.php?msg=loginFailed" ); 
			}
		}else{
			echo "aaa";
			Header( "HTTP/1.1 307 Temporary Redirect" ); 
			Header( "Location: login.php?msg=loginFailed" ); 
		
		}				  

		
			
			
	
		
		
?>