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
 

 if (isset($_SESSION['S241061_username'])){
	 
		//disabled autocommit
		mysqli_autocommit($conn, false);
	 
		$email = $_SESSION['S241061_username'];
		mysqli_select_db($conn,'reservations');
		
		$query=" SELECT count(*) FROM reservations WHERE email='".$email."' FOR UPDATE";
		$res = mysqli_query($conn, $query);	
		$row = mysqli_fetch_array($res,  MYSQLI_ASSOC);
		if ($row['count(*)']==0){
			$msg = "noReservation";
			mysqli_rollback($conn);
			mysqli_autocommit($conn, true);
			header('HTTP/1.1 307 temporary redirect');
			header('Location: personalPage.php?msg='.$msg);
			
		}
		$query2=" SELECT sum(assignedMin) FROM reservations FOR UPDATE";
		$res2 = mysqli_query($conn, $query2);	
		$row2 = mysqli_fetch_array($res2,  MYSQLI_ASSOC);
		$totAssigned = $row2['sum(assignedMin)'];
		if ($totAssigned < 180){
				//there are free minutes then don t have to recalculate
				$query3 = "DELETE from reservations WHERE email='".$email."'"; 
				$res3 = mysqli_query($conn, $query3);	
				if ($res3){
					$msg="deleteRegistration";
					//manual commit
					mysqli_commit($conn);
					mysqli_autocommit($conn, true);
					header('HTTP/1.1 307 temporary redirect');
					header('Location: personalPage.php?msg='.$msg);
				}else{
					//manual rollback
					mysqli_rollback($conn);
					$msg="queryErr";
				}
				
		}else{
				$query4 = "DELETE from reservations WHERE email='".$email."'"; 
				$res4 = mysqli_query($conn, $query4);	
				if ($res4){
					$query5 =" SELECT  sum(assignedMin),sum(requestedMin) FROM reservations FOR UPDATE";
					$res5 = mysqli_query($conn, $query5);	
					$row5 = mysqli_fetch_array($res5,  MYSQLI_ASSOC);
					$newTotAssigned = $row5['sum(assignedMin)'];
					$newTotRwquested = $row5['sum(requestedMin)'];
					if ($newTotRwquested <= 180){
						//total requested minutes is less or equal of 180 then all requests can be satysfied
						$key = array('id','email','requestedMin','assignedMin','timestamp');
						$query6=" SELECT * FROM reservations FOR UPDATE";
						if ($res6 = mysqli_query($conn, $query6)	){
							$flagErr = false;
							while($row6 = mysqli_fetch_array($res6,  MYSQLI_ASSOC)){
								$currentEmail = $row6['email'];
								$currentMin = $row6['requestedMin'];
								$queryUpdate = "UPDATE reservations SET assignedMin = '".$currentMin."' WHERE email='".$currentEmail."'";
								if (!$resUpdate = mysqli_query($conn, $queryUpdate)){
									$flagErr = true;
								}						
							}
							if ($flagErr == false){
								//manual commit
								$msg="deleteRegistration";
								mysqli_commit($conn);
							}else{
								//manual rollback
								mysqli_rollback($conn);
								$msg="queryErr";
							}
						}else{
							//manual rollback
								mysqli_rollback($conn);
								$msg="queryErr";
						}
					}else{
						//proportional update
						
							$key = array('id','email','requestedMin','assignedMin','timestamp');
							$query7 = "SELECT  * FROM reservations order by timestamp FOR UPDATE"; 
							$res7 = mysqli_query($conn, $query7);
							$tmpTot = 0;
							if ($res7){
								$flagErr=false;
								while ($row7 = mysqli_fetch_array($res7,  MYSQLI_ASSOC)){
									//update old reservations
									$newMin = $row7['requestedMin']*180;
									$newMin= floor($newMin/$newTotRwquested);
									$tmpTot += $newMin;
									$queryUpdate = "UPDATE reservations SET assignedMin = '".$newMin."' WHERE email='".$row['email']."'"; 
									if (!$resUpdate = mysqli_query($conn, $queryUpdate)){
											$flagErr = true;
									}
								}
								if ($flagErr==false){
									//manual commit
									$msg="deleteRegistration";
									mysqli_commit($conn);
								}else{
									//manual rollback
									mysqli_rollback($conn);
									$msg="queryErr";
								}
								//$newMin = 180 - $tmpTot;
								//$queryInsert = "INSERT INTO reservations (email, requestedMin, assignedMin)VALUES ('".$email."', '".$reqMin."', '".$newMin."');";
								//$resInsert = mysqli_query($conn, $queryInsert);	
							}else{
									//error in the query
									//manual rollback
									mysqli_rollback($conn);
									$msg="queryErr";
							}
					}
					
					
				}else{
					//manual rollback
					mysqli_rollback($conn);
					$msg="queryErr";
				}
			
			
		}
		
		
	}  
	
	 mysqli_autocommit($conn, true);
	 header('HTTP/1.1 307 temporary redirect');
	 header('Location: personalPage.php?msg='.$msg);


function recalculateMinutes(){


}

?>