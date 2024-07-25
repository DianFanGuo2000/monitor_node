
echo "Entering 'src' directory..."  
cd src  

  
echo "Executing 'make' command to compile sources..."  
make all

echo "Copying 'myapp' executable to the parent directory..."  
cp myapp ../work_station/myapp  
  

echo "Executing 'make clean' command to clean up build artifacts..."  
make clean  
  

echo "Exiting 'communication_monitor' directory and returning to the parent directory..."  
cd ..

