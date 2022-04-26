<!DOCTYPE html> 
<html> 
<head> 
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"> 
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="./css/style.css"/> 
 <link rel="stylesheet" href="./grid.css">

</head> 
<body>  
   <div class="row main" >
    <div class=" col l-6 c-0 login-img">
     <img src="http://dighuyniot.tech/demo/login/garden.jpg" class="garden-login" alt="garden">
     </div>
     <div class="login col l-0 m-12 c-12"><div class="child"><h1>SMART GARDEN</h1></div></div>  
   <div class="container  col l-6 c-12 "> 
       
       <form action='index.php' class="dangnhap " method='POST'> 
       Tên đăng nhập  <input type='text' name='username' /> 
       Mật khẩu  <input type='password' name='password' /> 
       <input type='submit' class="button" name="dangnhap" value='Đăng nhập' /> 
<!-- <a href='dangky.php' title='Đăng ký'>Đăng ký</a>  -->
       <p>"username:admin"</p>
       <p>"password:admin123"</p>
       <?php require 'xuly.php';?> 
       </form> 
      
  </div>
</div>
</body> 
</html>