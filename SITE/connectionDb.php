<?php 	
		//$conn = mysqli_connect('localhost', 's241061', 'nhanxswe','s241061'); 
		$conn = mysqli_connect('localhost', 'root', '','dp_luglio'); 
		if (!$conn) { 
			echo('Connect error (' . mysqli_connect_errno() . ') ' . mysqli_connect_error());
			echo"Database temporanely not avaiable, please retry more later";
			$_GET['msg'] = 'connectionFailed';
			
		}else{
			//echo 'Success... ' .mysqli_get_host_info($conn) . "\n"; 
			
		}
	
		

	
?> 