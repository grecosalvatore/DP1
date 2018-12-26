<?php
include 'session.php';
include 'checkCookie.php';
include'redirectNoCookies.php';
include'redirectNoHttps.php';
?>
<html>
<head>
<link href="mystyle.css" rel="stylesheet" type="text/css"/>
</head>
<body>

<div class="flex-container">
<header>
    <div class="titleHead"><a href="index.php"><img src="img/logo.png" id="logoImg" alt="logged" ></a><h1>BOOKING APPOIMENTS </h1></div>
  <div class="authenticationHead">
		<?php include 'authenticationHeader.php' ; ?> 
  </div>
</header>

<nav class="nav">
<ul class="menu-nav">
  <li class="menu-item"><a class="menu-link" href="index.php">HOME</a></li>
  <li class="menu-item-selected"><a class="menu-link" href="signup.php">SIGN UP</a></li>
  <li class="menu-item"><a class="menu-link" href="login.php">LOGIN</a></li>
   <?php
	include 'logged.php';
  ?>
</ul>


</nav>
<main class="main">
<noscript>
<div class="notActive" style="text-align:center;font-size:24;background-color:red;opacity:0.8; color:white;margin-top:2%;padding-left:5%;width:100%;height:5%;">Javascript is not active, some content may not be displayed correctly.</div>
</noscript>
<div class="pageTitle" ><h2>CREATE YOUR ACCOUNT<h2></div>
<script language="javascript" src="formvalidation.js">

</script>

 <div class="registration">
	 <?php if((isset($_SESSION["S241061_username"])) && (isset($_SESSION["S241061_password"]))){
	 ?>    <fieldset disabled="disabled"> 
	<?php   } 
		include 'errMsg.php';?>
  <noscript>  <fieldset disabled="disabled">  </noscript>
  <form id="registrationform" action="userRegistration.php" onsubmit="return signupValidation()" method="POST">
  

    <label for="lname">Email</label>
    <input type="text" id="userEmail" name="userEmail" placeholder="Enter Email..">

    <label for="lname">Password</label>
    <input type="password" id="userPsw" name="userPsw" placeholder="Enter Password..">
	
	<label for="lname">Confirm Password</label>
    <input type="password" id="userConfirmPsw" name="userConfirmPsw" placeholder="Confirm Password..">
  
    <input type="submit" value="Submit">
  </form>
  <?php if((isset($_SESSION["S241061_username"])) && (isset($_SESSION["S241061_password"]))){ ?>
	You cannot signup another account while are logged <a class="linkOnPage" href="logout.php"> <b>Logout</b></a> 
	<?php   }else { ?> Already have an account <a class="linkOnPage" href="login.php"> <b>Login</b></a> <?php } ?>
</div>



</main>

<footer>Copyright Greco Salvatore Politecnico of Turin &copy; </footer>
</div>

</body>
</html>

