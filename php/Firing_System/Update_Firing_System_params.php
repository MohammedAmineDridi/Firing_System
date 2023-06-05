<?php
// Database connection parameters
$host = 'localhost';
$username = 'root';
$password = 'Aminedridi11';
$database = 'Firing_system';

// Create a connection to the database
$connection = mysqli_connect($host, $username, $password, $database);

// Check if the connection was successful
if (!$connection) {
    die("Connection failed: " . mysqli_connect_error());
}

// Retrieve the values from a form or other data source
$k = $_POST['k'];
$L0 = $_POST['L0'];
$a = $_POST['a'];
$b = $_POST['b'];
$m = $_POST['m'];

// Construct the UPDATE query
$query = "UPDATE firing_system_params SET k = '$k', L0 = '$L0', a = '$a', b = '$b', m = '$m' WHERE id = 1";

// Execute the query
$result = mysqli_query($connection, $query);

// Check if the UPDATE query was successful
if ($result) {
    echo "Record updated successfully.";
} else {
    echo "Error updating record: " . mysqli_error($connection);
}

// Close the database connection
mysqli_close($connection);
?>