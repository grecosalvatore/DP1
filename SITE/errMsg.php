<?php
function sanitizeString($var) {
	$var = strip_tags($var);
	$var = htmlentities($var);
	$var = stripcslashes($var);
	//$var = mysql_real_escape_string($conn,$var);
	return $var;
}


	 if (isset($_GET['msg'])){
		$typeError = sanitizeString($_GET['msg']);
		if ($typeError == 'loginFailed'){
			$msg = "Username or password are not correct";
				echo"<div class='divErrMesg'>".$msg."</div>";
		} elseif ($typeError == 'registrationFailed'){
				$msg = "Error in the registration, please retry";
				echo"<div class='divErrMesg'>".$msg."</div>";
		} elseif ($typeError == 'connectionFailed'){
				$msg = "Database temporanely not avaiable";	
				echo"<div class='divErrMesg'>".$msg."</div>";
		} elseif ($typeError == 'alreadySignup'){
				$msg = "You are already registered";	
				echo"<div class='divErrMesg'>".$msg."</div>";
		} elseif ($typeError == 'queryErr'){
				$msg = "An error as occurred, please retry";	
				echo"<div class='divErrMesg'>".$msg."</div>";
		} elseif ($typeError == 'sessError'){
				$msg = "Error exipred";	
				echo"<div class='divErrMesg'>".$msg."</div>";
		}elseif ($typeError == 'notAuthorized'){
				$msg = "Login to access your personal page";
				echo"<div class='divErrMesg'>".$msg."</div>";
		}
		elseif ($typeError == 'alreadyReserved'){
				$msg = "You already have a reservation";
				echo"<div class='divErrMesg'>".$msg."</div>";
		}
		elseif ($typeError == 'notMinutesAvaiable'){
				$msg = "There aren't free minutes avaiable";
				echo"<div class='divErrMesg'>".$msg."</div>";
		}
		elseif ($typeError == 'deleteRegistration'){
				$msg = "You have succesfully deleted your reservation";
				echo"<div class='divErrMesg'>".$msg."</div>";
		}
		elseif ($typeError == 'noReservation'){
				$msg = "You don't have any reservation";
				echo"<div class='divErrMesg'>".$msg."</div>";
		}
			
			
			
		//unset($_GET['msg']);
	
	 }								




?>