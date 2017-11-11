<?php
//This code refreshes the page every $sec
$page = $_SERVER['PHP_SELF'];
$sec = "1";
?>
<html>
    <head>
    <meta http-equiv="refresh" content="<?php echo $sec?>;URL='<?php echo $page?>'">
	<link rel="stylesheet" type="text/css" href="interface.css">
    </head>
    <body>
<!--.......................................................................................................-->
<?php
//connect to database
$con=mysqli_connect("localhost","root","","esp");// server, user, pass, database

// Check connection
if (mysqli_connect_errno()) {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
}
//grab the table out of the database
$result = mysqli_query($con,"SELECT * FROM `esptable`");//table select
/*------------------------------------------------------------------------------------------------------------*/
//Title
echo "<div> UTSVT Telemetry Demo <br> </div>";
//Tables
echo "<table border='2'>
<tr>
<th>id</th>
<th>SENSOR</th>
<th>SENSOR1</th>
</tr>";
//loop through the table and print the data into the table
while($row = mysqli_fetch_array($result)) {
  echo "<tr>";
  $unit_id = $row['id'];
  echo "<td>" . $row['id'] . "</td>";
  echo "<td>" . $row['SENSOR'] . "</td>";
  echo "<td>" . $row['SENSOR1'] . "</td>";
}//while

echo "</table>";

    ?>
    