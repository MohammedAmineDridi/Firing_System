<?php
// Database connection parameters
$host = 'localhost';
$username = 'root';
$password = 'Aminedridi11';
$database = 'Firing_system';

// Create a connection to the database
$connection = new mysqli($host, $username, $password, $database);

// Check if the connection was successful
if ($connection->connect_error) {
    die("Connection failed: " . $connection->connect_error);
}

// Fetch data from the database
$query = "SELECT * FROM firing_system_params";
$result = $connection->query($query);

// Process the fetched data
$data = array();
if ($result->num_rows > 0) {
    while ($row = $result->fetch_assoc()) {
        $data[] = $row;
    }
}

// Set the response headers for CORS
header('Access-Control-Allow-Origin: *');
header('Content-Type: application/json');

// Output the response as JSON
echo json_encode($data);

// Close the database connection
$connection->close();
?>