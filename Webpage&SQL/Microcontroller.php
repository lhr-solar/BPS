<?php

//grab variables off the URL
foreach($_REQUEST as $key => $value)
{
if($key =="unit"){
$unit = $value;
}

if($key =="sensor"){
$sensor = $value;
}
if($key =="sensor1"){
$sensor1 = $value;
}	
}
/*-----------------------------------------------------------------------------------------------------*/
//Connection to SQL database
$con=mysqli_connect("localhost","root","","esp");// server, user, pass, database

if (mysqli_connect_errno()) {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
}
/*-----------------------------------------------------------------------------------------------------*/
//update sensor values in database
mysqli_query($con,"UPDATE esptable SET SENSOR = $sensor
WHERE id=$unit");

mysqli_query($con,"UPDATE esptable SET SENSOR1 = $sensor1
WHERE id=$unit");
/*-----------------------------------------------------------------------------------------------------*/
//Time/Time zone
date_default_timezone_set('America/Chicago');
$t1 = date("gi");//many different possible formats, but this gives 24 hr format, and returns 1:23 as 123
/*-----------------------------------------------------------------------------------------------------*/
//pull out the table
$result = mysqli_query($con,"SELECT * FROM `esptable`");
/*-----------------------------------------------------------------------------------------------------*/
//loop through the table and filter out data for this unit id
while($row = mysqli_fetch_array($result)) {
if($row['id'] == $unit){
$d1 = $row['SENSOR'];
$d2 = $row['SENSOR1'];
echo "_t1$t1##_d1$d1##_d2$d2##";
}
}
?>
<!--...................................................................................................-->