#!/bin/bash  
  
sudo chmod +x myapp
  
sudo ./myapp configure1.json listen ens33 res.json &
sudo ./myapp configure1.json test &