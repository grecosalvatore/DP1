<?php 
	function sanitizeString($conn,$var) {
		$var = strip_tags($var);
		$var = htmlentities($var);
		$var = stripcslashes($var);
		$var = mysqli_real_escape_string($conn,$var); 
		return $var;
	}
	
	
			session_start();

		if ((isset($_POST["userEmail"])) && (isset($_POST["userPsw"]))){
				
		
		
		
			//connection on the db
			include 'connectionDb.php';
	
			mysqli_select_db($conn,'users');
			//mysqli_autocommit($conn, false); // disable autocommit
			
			//input validation
			$email =sanitizeString($conn,$_POST["userEmail"]);
			$psw = sanitizeString($conn,$_POST["userPsw"]);
	
			//hash psw with md5
			$psw=md5($psw);
			
			//check if the email is already recorded in the database 
			$query = "SELECT count(*) FROM users WHERE email='".$email."' FOR UPDATE"; 
			$res = @mysqli_query($conn, $query); 
			$row = mysqli_fetch_array($res, MYSQLI_NUM);
			
			$count= $row[0];
			
			if ($count == 0){
				//use md5 to save hashed password
				$query = "INSERT INTO users ( email, password) VALUES ( '".$email."','". $psw."')";
				
		
		
				if ( @mysqli_query($conn,$query)){
					$_SESSION["S241061_username"]=$email; 
					$_SESSION["S241061_password"]=$psw;
					$_SESSION['S241061_time']=time();
					//mysqli_commit($conn);
					Header( "HTTP/1.1 307 Temporary Redirect" ); 
					Header( "Location: personalPage.php" );
				}else{
					//mysqli_rollback($conn);
					Header( "HTTP/1.1 307 Temporary Redirect" ); 
					Header( "Location: signup.php?msg=registrationFailed" ); 
				}
			}else {
				//the email is already recorded in the database
				//mysqli_rollback($conn);
				echo"you are already registered";
				Header( "HTTP/1.1 307 Temporary Redirect" ); 
				Header( "Location: signup.php?msg=alreadySignup" ); 

			}
			//mysqli_autocommit($conn, true); // activate autocommit
		}else{
			Header( "HTTP/1.1 307 Temporary Redirect" ); 
			Header( "Location: signup.php?msg=registrationFailed" ); 
		}
		
		
		
			
	
		
		
?>