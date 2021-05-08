int sig1;
int sig2;
double sig_diff;
int min_val = -50;
double max_val = 50;
int threshold = -20;
int hyst = 5;
double go_sig = 0;
double break_sig = 0;
int min_sensor = 800;


//int idx_val = 0;

//float vals[] = {0,0,0,0,0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("sig_diff:, min:, max:, zero:");
}

void loop() {
 
  sig1 = analogRead(A1);
  sig2 = analogRead(A0);

  sig_diff = sig2-sig1;

//  // filter signal
//  idx_vals = (idx_vals + 1) % 5
//  vals[idx_vals] = go_sig

  
  //sig_diff = pow((sig_diff/1000), 10);

  // Person must be standing on both sensors to control the board
  if (sig1 > min_sensor || sig2 > min_sensor) {
    go_sig = sig_diff - threshold-hyst;
    break_sig = sig_diff - threshold+hyst;
  
    // Truncate
    go_sig = max(go_sig, 0);
    go_sig = min(go_sig, max_val);
    break_sig = max(break_sig, min_val);
    break_sig = min(break_sig, 0);
    // break_sig = abs(break_sig);
    
  } else {
    go_sig = 0;
    break_sig = 0;
  }

  //Visualisation
  Serial.print(go_sig);
  Serial.print(",");
  Serial.print(break_sig);
  Serial.print(",");
  Serial.print(min_val);
  Serial.print(",");
  Serial.print(max_val);
  Serial.print(",");
  Serial.println(0);
  //Serial.println(sig1);
 
}
