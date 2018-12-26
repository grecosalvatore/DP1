<?php
	include 'connectionDb.php';

	mysqli_select_db($conn,'reservations');
	$key = array('id','email','requestedMin','assignedMin','timestamp');
	$query = "SELECT * FROM reservations order by timestamp"; 
	$res = @mysqli_query($conn, $query); 
	$hour=14;
	$min=00;
	$precH = $hour;
	$precM = $min;
	if ($res){
		while ($row = mysqli_fetch_array($res,  MYSQLI_BOTH)){
			
			if (strlen($precM)!=2){
				$precM = "0".$precM;
			}
			$tmpStart = $precH.":".$precM ;
			
			$tmpHour =  floor( ($min+$row['assignedMin'])/60);
			$tmpMin =   ($min+$row['assignedMin'])%60;
			$hour += $tmpHour;
			$min = $tmpMin;
			if (strlen($min) ==2){
				$tmpMin = $min;
			}else{
					$tmpMin="0".$min;
			}
			
			$precH = $hour;
			$precM = $tmpMin;		
			
			?>
			<tr class="previewContent">
				<?php
				echo "<th>" .  $row['email'] . "</th>";
				echo "<th>" .  $row['requestedMin'] . "</th>";
				echo "<th>" .  $row['assignedMin'] . "</th>";
				echo "<th>" . $tmpStart. "</th>";
				echo "<th>" .$hour.":".$tmpMin. "</th>";
				?>	
			</tr>
			<?php
		}
	}else{
			//error in the query
	}
	
	$key = array('assigned','requested');
	$query = "SELECT sum(assignedMin),sum(requestedMin)   FROM reservations"; 
	$res = @mysqli_query($conn, $query); 
	if ($res){
		$row = mysqli_fetch_array($res,  MYSQLI_BOTH);
		$totAssigned = $row['sum(assignedMin)'];
		$totRequested = $row['sum(requestedMin)'];
		echo" <b>Total assigned minutes: <font color='red'>".$totAssigned."</font><br>";
		echo" <b>Total requested minutes: <font color='red'>".$totRequested."</font><br>";
	}else{
			//error in the query
			echo"errorr";
	}
	
?>	