<?php
	function sanitizeString($conn,$var) {
		$var = strip_tags($var);
		$var = htmlentities($var);
		$var = stripcslashes($var);
		$var = mysqli_real_escape_string($conn,$var); 
		return $var;
	}


	session_start();
	include 'connectionDb.php';
	
	if ((isset($_SESSION["S241061_username"])) && (isset($_SESSION["S241061_password"]))){	
		if(isset($_POST["res"])){
			//disable autocommit
			mysqli_autocommit($conn, false);
			$email = $_SESSION['S241061_username'];
			$reqMin=sanitizeString($conn,$_POST["res"]);
			
			$query = "SELECT count(*) from reservations WHERE email='".$email."' FOR UPDATE"; 
			$result = mysqli_query($conn, $query);	
			$row = mysqli_fetch_array($result,  MYSQLI_BOTH);
			echo $row['count(*)'];
			if ($row['count(*)'] > 0){
				//already reserved
				mysqli_rollback($conn);
				//enable autocommit
				mysqli_autocommit($conn, true);
				header('HTTP/1.1 307 temporary redirect');
				header('Location: personalPage.php?msg=alreadyReserved');
				
			}
			
			mysqli_select_db($conn,'reservations');
			$query = "SELECT sum(assignedMin),sum(requestedMin) from reservations FOR UPDATE"; 
			$res = mysqli_query($conn, $query);	
			$row = mysqli_fetch_array($res,  MYSQLI_BOTH);
			$totRequested = $row['sum(requestedMin)'];
			if ( $row['sum(assignedMin)'] == NULL){
				//there are not reservation
				if ($reqMin <= 180){
					$query = "INSERT INTO reservations (email, requestedMin, assignedMin)VALUES ('".$email."', '".$reqMin."', '".$reqMin."');";
					if ($res = mysqli_query($conn, $query)){
						mysqli_commit($conn);
					}else{
						mysqli_rollback($conn);
						$msg="queryErr";
					}
				}
			}else{
				
					if ($row['sum(assignedMin)'] == 180){
						//there are not free minutes
						$msg="notMinutesAvaiable";
					}else{
						//there already are other reservation
						if (($reqMin + $row['sum(assignedMin)']) <= 180){
							//free minutes are more then requested ones
							$query = "INSERT INTO reservations (email, requestedMin, assignedMin)VALUES ('".$email."', '".$reqMin."', '".$reqMin."');";
							if ($res = mysqli_query($conn, $query)){
								mysqli_commit($conn);
							}else{
								mysqli_rollback($conn);
								$msg="queryErr";
							}
						}else{
							//free minutes are less then requested ones
							$totRequested += $reqMin; //sum of requested minutes
							$key = array('id','email','requestedMin','assignedMin','timestamp');
							$query = "SELECT * FROM reservations order by timestamp"; 
							$res = mysqli_query($conn, $query);
							$tmpTot = 0;
							if ($res){
								$flagErr = false;
								while ($row = mysqli_fetch_array($res,  MYSQLI_BOTH)){
									//update old reservations
									$newMin = floor(( $row['requestedMin']*180)/$totRequested);
									$tmpTot += $newMin;
									$queryUpdate = "UPDATE reservations SET assignedMin = '".$newMin."' WHERE email='".$row['email']."'"; 
									if (!$resUpdate = mysqli_query($conn, $queryUpdate)){
											$flagErr = true;
									}
								}
								$newMin = 180 - $tmpTot;
								$queryInsert = "INSERT INTO reservations (email, requestedMin, assignedMin)VALUES ('".$email."', '".$reqMin."', '".$newMin."');";
								if (!$resInsert = mysqli_query($conn, $queryInsert)){
									$flagErr = true;
								}
								
								if ($flagErr == false){
									mysqli_commit($conn);
								}else  {
									mysqli_rollback($conn);
									$msg="queryErr";
								}
								
							}else{
								mysqli_rollback($conn);
								$msg="queryErr";
							}
							
						}
					}
			}	
		}else{
			$msg = "sessError";
		}
	}
		
	 //enable autocommit
	 mysqli_autocommit($conn, true);
	 header('HTTP/1.1 307 temporary redirect');
	 header('Location: personalPage.php?msg='.$msg);
	 
?>