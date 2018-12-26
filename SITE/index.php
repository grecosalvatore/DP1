  <?php
	include 'homeSession.php';
	include'redirectNoHttps.php';
	include 'checkCookie.php';
	
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
<script></script>

<nav class="nav">
<ul class="menu-nav">
  <li class="menu-item-selected"><a class="menu-link" href="index.php">HOME</a></li>
  <li class="menu-item"><a class="menu-link" href="signup.php">SIGN UP</a></li>
  <li class="menu-item"><a class="menu-link" href="login.php">LOGIN</a></li>
     <?php
	include 'logged.php';
  ?>

</ul>

</nav>
<main class="main">
<script></script>
<noscript>
<div class="notActive" style="text-align:center;font-size:24;background-color:red;opacity:0.8; color:white;margin-top:2%;padding-left:5%;width:100%;height:5%;">Javascript is not active, some content may not be displayed correctly.</div>
</noscript>
<?php
		if (get_cookie_enabled()==false){
	?>
		<div class="notActive" style="text-align:center;font-size:24;background-color:red;opacity:0.8; color:white;margin-top:2%;padding-left:5%;width:100%;height:5%;">Your browser have disabled cookie, active it if want use the site</div>
		<?php	}
	?>
<div class="pageTitle" ><h2>HOMEPAGE<h2></div>

<div class="preview">Welcome, here you can see the assigned reservations:
</div>
<div class="itemsPreview">
<table id="tableItemsPreviw">
  <tr id="tableHead">
    <th>EMAIL</th> 
	<th>REQUESTED MINUTES</th>
    <th>ASSIGNED MINUTES</th>
	<th>STARTING HOUR</th>
	<th>END HOUR</th>
  </tr>
  <?php include 'homePreview.php'; ?>
 </table>
</div>

</main>

<footer>Copyright Greco Salvatore Politecnico of Turin &copy; </footer>
</div>

</body>
</html>

