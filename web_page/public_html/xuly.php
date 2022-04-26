<?php
// session_start();

//Khai báo sử dụng session

//Khai báo utf-8 để hiển thị được tiếng việt
// ('Content-Type: text/html; charset=UTF-8');
//Xử lý đăng nhập
if (isset($_POST['dangnhap']))
{
//Kết nối tới database
include('connect.php');
  
//Lấy dữ liệu nhập vào
$username = addslashes($_POST['username']);
$password = addslashes($_POST['password']);
  
//Kiểm tra đã nhập đủ tên đăng nhập với mật khẩu chưa
if (!$username || !$password) {
echo "Vui lòng nhập đầy đủ tên đăng nhập và mật khẩu. <a href='javascript: history.go(-1)'>Trở lại</a>";
exit;
}
  
// mã hóa pasword
// $password = md5($password);
  
//Kiểm tra tên đăng nhập có tồn tại không
$query = "SELECT username, password FROM users WHERE username='$username'";

$result = mysqli_query($connect, $query) or die( mysqli_error($connect));

if (!$result) {
echo "Tên đăng nhập hoặc mật khẩu không đúng!";
} else {
echo "Đăng nhập thành công!";
}
  
//Lấy mật khẩu trong database ra
$row = mysqli_fetch_array($result);
  
//So sánh 2 mật khẩu có trùng khớp hay không
if ($password != $row['password']) {
echo "Mật khẩu không đúng. Vui lòng nhập lại. <a href='javascript: history.go(-1)'>Trở lại</a>";
exit;
}
  
//Lưu tên đăng nhập
$_SESSION['username'] = $username;
// redirect("http://dighuyniot.tech/demo/IOT/index.html");
echo '<script> window.location="http://dighuyniot.tech/demo/IOT/index.html";</script>';
//echo "<b>" .$username . "</b>. Đăng nhập thành công.Đến <a href='http://dighuyniot.tech/demo/IOT/index.html'>TRANG CHỦ</a>";
die();
$connect->close();
}
exit;
?>