<?php


	if((isset($_SESSION["S241061_username"])) && (isset($_SESSION["S241061_password"]))){
			 ?>
			 <a href="personalPage.php"><img src="img/logged.png" id="loggedImg" alt="logged" "></a>
	<?php		
			echo $_SESSION["S241061_username"]; ?>
			<br><a  href="logout.php" class="logout-link">Logout</a>
			
	<?php
	}else{ 
	?>
		<a  href="login.php" class="login-link">Login</a>
	<?php
	}

	
?>
		
		


<?php

?>