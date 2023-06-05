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
$query = "SELECT * FROM bielle_manivelle_system_params t1 JOIN firing_system_params t2 ON t1.id = t2.id";
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

// step 1 : get Delta_X and Delta_Y

// step 1 : Output the response as JSON
$k = $data[0]['k'];
$L0 = $data[0]['L0'];
$a = $data[0]['a'];
$b = $data[0]['b'];
$m = $data[0]['m'];
$r = $data[0]['r'];
$l = $data[0]['l'];
$n = $data[0]['n'];
$g = 9.81;

// step 2 : get params : DX et DY from url ... ex : http://example.com/page.php?param1=value1&param2=value2
$delta_x;
$delta_y; 
if (isset($_GET['DX'])) {
    $delta_x = $_GET['DX'];
}
if (isset($_GET['DY'])) {
    $delta_y = $_GET['DY'];
}

// step 3 : calculation of (Vi,Teta angle) : + test (k = 5/L0 = 100/a = 210/b = 60/ m = 0.1)

// calcul params .

function radiansToDegrees(float $radians) 
{
    return $radians * (180.0 / M_PI);
}

$l0 = sqrt( (pow($L0,2)) - (pow(($b/2),2)) );  // slinghsot bullet idle state distance . 
$l1 = $delta_x; // exmple : every 1m in DX -> l1+= 1cm /  l1 (elongation center distance) prop with distance between src & target .
$delta_x += ($l1/100); // update the new DX (after slinghsot elongation) ( l1/100 : bcz 'l1' in cm & DX in 'm' )
$L1 = sqrt( (pow(($l0+$l1),2)) + pow(($b/2),2) ); // elongation distance of 2 threads
$alpha = atan(($b/2)/($l0+$l1));
$p = 2*$k*$L1*cos($alpha); // the force needed to elongate the slinghsot threads to 'l1' distance .
$vi = sqrt( ((float)($k/$m)) * pow(($L1/100),2) );  // L1/100 : from (cm) to (m) , bcz : vi(m/s)
$angle;
    if ( $delta_y != 0 ) {
        $angle = radiansToDegrees( ( ( acos( (( ($g*pow($delta_x,2))/(pow($vi,2)) ) - $delta_y) / (sqrt( pow($delta_y,2) + pow($delta_x,2) )) )/2 ) + (atan( ($delta_x/$delta_y) )) )/2 ) ;  
    } else {
        $angle = radiansToDegrees( asin( (2*$g*($delta_x/2))/pow($vi,2) ) / 2 );
    }

function Manivelle_Rotation_Angle(float $r,float $l,float $x) // x = $l1 from previous calculations ....
{
    $alpha_angle = 0;
    $a = pow($r,2)/(2*$l);
    $b = $r;
    $c = $x-$r-( (pow($r,2)) / (2*$l) );
    $delta = pow($b,2)-(4*$a*$c);
    if ($delta < 0){
        echo "error";
    } else if ($delta == 0){
        // unique solution 
        $x1 = -$b/(2*$a);
    } else {
        // Delta > 0 : 2 solutions .
        $x1 = (-$b-sqrt($delta))/(2*$a);
        $x2 = (-$b+sqrt($delta))/(2*$a);
        if ($x1 >= -1 && $x1 <= 1) {
            $alpha_angle = radiansToDegrees(acos($x1));
        } else if ($x2 >= -1 && $x2 <= 1) {
            $alpha_angle = radiansToDegrees(acos($x2));
        }   
    }
    return $alpha_angle;
}




// return json result 

// Set the response headers for CORS
header('Access-Control-Allow-Origin: *');
header('Content-Type: application/json');

// Output the response as JSON
$Result = array(); // result array
array_push($Result,$angle); 

$rotation_angle_manivelle = Manivelle_Rotation_Angle($r,$l,($l1/10));
array_push($Result,$rotation_angle_manivelle);

// json response
echo json_encode($Result);

// Close the database connection
$connection->close();
?>