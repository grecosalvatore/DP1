//function: return true if the string is a valid email
function emailValidation(email) {
  var reg = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;
  if (!reg.test(email)) return false; else return true;
}

//function: return true if the string is a valid psw (at least one number and at least one character)
function passwordValidation(psw) {
	var valid = true;
	var reg =/^[a-zA-Z0-9\_\*\-\+\!\?\,\:\;\.\xE0\xE8\xE9\xF9\xF2\xEC\x27]{3,15}/;
	if (!reg.test(psw)) {
		valid = false; 
	}
	//at least one number
	var reg2 =/[0-9]/;
	if (!reg2.test(psw)) {
		valid = false; 
	}
	//at least one character
	var reg3 =/[a-z]/;
	if (!reg3.test(psw)) {
		valid = false; 
	}
	
	var reg4 =/[A-Z]/;
	if (!reg4.test(psw)) {
		valid = false; 
	}
	return valid;
}


// check login form
function loginValidation(){
	
var email = document.getElementById("loginform").elements.namedItem("logUserEmail").value;
var psw = document.getElementById("loginform").elements.namedItem("logUserPsw").value;

 

if ((email == "") || (email == "undefined")) {
	//if i am there the field email is empty
   alert("Please enter Email");
   //document.getElementById("loginform").userEmail.focus();
   return false;
}else{
	if (emailValidation(email) == false){
		//if i am there the field email is not empty but is not valid
		alert("Please enter a valid Email");
		//document.getElementById("loginform").userEmail.focus();
		return false;
	}else{
		//if i am there the email field is potentially good then can check other fields
		if ((psw == "") || (psw == "undefined")) {
			alert("Please enter Password");
			//document.getElementById("loginform").userPsw.focus();
			return false;
		}else{
			
			//password must contain at least one alphabetic character and one numeric character 
			if (passwordValidation(psw) == false){
				alert("Please enter valid Password");
				//document.getElementById("loginform").userPsw.focus();
				return false;
			}
			else{	
			//if i am here can call the php page for the check server side
				// document.getElementById("loginform").action = "userLogin.php";
				 //document.getElementById("loginform").elements.namedItem('btnSub').submit();
				 return true;
				
			
			}

		
	}
	}
	}


}


//check signup form
function signupValidation(){
	
var email = document.getElementById("registrationform").elements.namedItem("userEmail").value;
var psw = document.getElementById("registrationform").elements.namedItem("userPsw").value;
var confirmPsw = document.getElementById("registrationform").elements.namedItem("userConfirmPsw").value;
var name = document.getElementById("registrationform").elements.namedItem("userPsw").value;
var surname = document.getElementById("registrationform").elements.namedItem("userConfirmPsw").value;


if ((email == "") || (email == "undefined")) {
	//if i am there the field email is empty
   alert("Please enter Email");
  // document.getElementById("registrationform").userEmail.focus();
   return false;
}else{
	if (emailValidation(email) == false){
		//if i am there the field email is not empty but is not valid
		alert("Please enter a valid Email");
		//document.getElementById("registrationform").userEmail.focus();
		return false;
	}else{
		//if i am there the email field is potentially good then can check other fields
		if ((psw == "") || (psw == "undefined")) {
			alert("Please enter Password");
			//document.getElementById("registrationform").userPsw.focus();
			return false;
		}else{
			
			//password must contain at least one alphabetic character and one numeric character 
			if (passwordValidation(psw) == false){
				alert("Please enter valid Password");
				//document.getElementById("registrationform").userPsw.focus();
				return false;	
			}
			else{	
				//if i am there the password is valid then have to check if password and confirm password are equal
				if (psw != confirmPsw){
						//if i am there psw and confirm psw are different
						alert("Password and Confirm Password are not equal");
						//document.getElementById("registrationform").userPsw.focus();
						//location = "signup.php";
						return false;
				}else{
					
					//if i am here can call the php page for the check server side
					// document.getElementById("registrationform").action = "userRegistration.php";
					// document.getElementById("registrationform").submit();
					 return true;
				}
			}

		
	}
	}
	}
}	

// check reservatio form
function reservationValidation(){
		
	var res = document.getElementById("resForm").elements.namedItem("res").value;

	if ((res == "") || (res == "res")) {
			alert("Please enter minutes requested for the appointement");
			//document.getElementById("thrform").userPsw.focus();
			return false;
	}else{
		if (res <= 0) {
			//document.getElementById("thrform").elements.namedItem("thr").setCustomValidity("Thr must be greater then 1,00");
			alert("min minutes : 1");
			//document.getElementById("thrform").elements.namedItem("thr").userPsw.focus();
			return false;
		}else{
			if(thr > 180){
					//document.getElementById("thrform").elements.namedItem("thr").setCustomValidity('Thr max value is  3.402823466E+38');
					alert("max minutes = 180");
					//document.getElementById("thrform").userPsw.focus();
					return false;
			}
			else{
			//document.getElementById("thrform").action = "userThr.php";
			//document.getElementById("thrform").submit();
			return true;
			}
		}
	}
}
	
function setSignupLoginForm(parameter){
	//document.getElementById("registrationform").disabled = parameter;
	document.getElementById("loginform").disabled = parameter;
}

function disabledPrenotation(parameter){
	document.getElementById("resForm").disabled = parameter;
	document.getElementById("delbut").disabled = !parameter;
}

