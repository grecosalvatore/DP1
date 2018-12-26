<?php
	include 'session.php';
	include 'checkCookie.php';
	include'redirectNoCookies.php';
	include'redirectNoHttps.php';
	include 'redirectNoAuthentication.php';
	include 'connectionDb.php';
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
  <li class="menu-item"><a class="menu-link" href="signup.php">SIGN UP</a></li>
  <li class="menu-item"><a class="menu-link" href="login.php">LOGIN</a></li>
	<li class="menu-item-selected"><a class="menu-link" href="personalPage.php">PROFILE</a></li>
</ul>

</nav>

<main class="main">
<noscript>
<div class="notActive" style="text-align:center;font-size:24;background-color:red;opacity:0.8; color:white;margin-top:2%;padding-left:5%;width:100%;height:5%;">Javascript is not active, some content may not be displayed correctly.</div>
</noscript>
<div class="pageTitle" ><h2>YOUR PROFILE<h2></div>

<script language="javascript" src="formvalidation.js"></script>

 <div class="resDiv" >
 <noscript>  <fieldset disabled="disabled">  </noscript>
  <?php   
	include 'errMsg.php';
	?>
	<form id="resForm"   action="userReservation.php" onsubmit="return reservationValidation()" method="POST" > 
    <label for="lname" id="resLabel">Reequested minutes reservation:   </label>
    <input type="number" id="res"  name="res" min="1" max="180" step="1" placeholder="Enter minutes.."   lang="en-150">
	
  
    <input type="submit" id="resSubmit" value="SET">
	
  </form>
  <form id="delForm"   action="deleteReservation.php"  method="POST" > 
 
	<input type="submit" id="delBut" value="DEL" >
  </form>
  <div class="resDiv" id="showPren">
   <?php include'showPrenotation.php';?>
  </div>

 
 </div>
 
	
 
</main>

<footer>Copyright Greco Salvatore Politecnico of Turin &copy; </footer>
</div>

</body>
</html>
