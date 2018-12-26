<?php
	include 'connectionDb.php';
	
	if (isset($_SESSION['S241061_username'])){
		$email = $_SESSION['S241061_username'];
		$disabledReservation = false;
		mysqli_select_db($conn,'reservations');
		$query = "SELECT count(*) FROM reservations WHERE email='".$email."'"; 
		$res = mysqli_query($conn, $query);
		
		if ($res){
			$row = mysqli_fetch_array($res,  MYSQLI_BOTH);
			if ($row['count(*)'] > 0){
					//the user already have inserted a request of prenotation
					$disabledReservation = true;
					$query = "SELECT timestamp FROM reservations WHERE email='".$email."'"; 
					$res = mysqli_query($conn, $query);
					$row = mysqli_fetch_array($res,  MYSQLI_BOTH);
					
					$queryTotal = "SELECT sum(assignedMin) FROM reservations WHERE timestamp <= '".$row['timestamp']."'"; 
					
					
					$resFindMin = mysqli_query($conn, $queryTotal);
					$rowMin = mysqli_fetch_array($resFindMin,  MYSQLI_BOTH);
					
					
					$totMin = $rowMin['sum(assignedMin)'];
					$key = array('id','email','requestedMin','assignedMin','timestamp');
					$queryInner = "SELECT * FROM reservations WHERE email='".$email."'"; 
					$resInner = mysqli_query($conn, $queryInner);
					$rowInner = mysqli_fetch_array($resInner,  MYSQLI_BOTH);
					echo"<div class='showReservation'><strong>";
					echo"<div class='textPren'><font color=red ><b>Your Reservation: &emsp;</font>";
					echo"<font color=#006070  ><b>";
					echo"Requested Minutes:";
					echo $rowInner['requestedMin'];
					echo"&emsp;Assigned Minutes:";
					echo $rowInner['assignedMin'];
					$tmpHour = (floor($totMin/60));
					$tmpHour += 14;
					$tmpMin = $totMin%60;
					if (strlen($tmpMin!=2)){
							$tmpMin = "0".$tmpMin;
					}
					$endHour = $tmpHour.":".$tmpMin;
					
					
					$tmpH = (floor(($totMin-$rowInner['assignedMin'])/60));
					$tmpH += 14;
					$tmpM = ($totMin-$rowInner['assignedMin'])%60;
					
					if (strlen($tmpM)!=2){
							$tmpM = "0".$tmpM;
					}
					$startHour = $tmpH.":".$tmpM;
					echo "&emsp;Start Hour: ";
					echo $startHour;
					
					
					echo "&emsp;End Hour: ";
					echo $endHour;
					
					echo"</font></div></div>";
					 
			}else{
					//the user not already have inserted a reservation
			}
		}else{
				echo"error";//error in the query
		}
	}







?>