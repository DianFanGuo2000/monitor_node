#!/bin/bash  
  
sudo chmod +x myapp
  
sudo ./myapp configure2.json listen ens33 res.json &
sudo ./myapp configure2.json test &