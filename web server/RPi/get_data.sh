#!/bin/bash
python3 /var/www/html/get_data.py > /var/www/html/current_data.txt
date >> /var/www/html/log_data.txt
python3 /var/www/html/get_data.py >> /var/www/html/log_data.txt
