
<?php
define('DBSERVER', 'localhost'); // Database server
define('DBUSERNAME', 'dighuyni_smartgarden'); // Database username
define('DBPASSWORD', '12345678'); // Database password
define('DBNAME', 'dighuyni_smartgarden'); // Database name
 
/* connect to MySQL database */
$connect = mysqli_connect(DBSERVER, DBUSERNAME, DBPASSWORD, DBNAME);
 
// Check db connection
if($connect === false){
    die("Error: connection error. " . mysqli_connect_error());
}
?>
