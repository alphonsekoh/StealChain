# anti-theft-alarm
Anti theft alarm system coded in Arduino
---
### Project set up steps:
1. Clone project  
```git clone https://github.com/hanyi97/chatbot.git```

1. Checkout main branch  
```git checkout main```

1. Create and checkout feature branch  
```git checkout -b feature/<featurename>```

4. Push your feature branch to GitHub  
```git push -u origin feature/<featurename>```

### Basic workflow:
1. Checkout your feature branch  
```git checkout feature/<featurename>```

1. Code and test your feature  

2. Add your changes to the feature branch (best practice to add the files that you modified only)  
```git add <file name>```

4. Commit your changes  
```git commit -m “<commit message>”```  
or  
```git commit``` Press enter and type the summary and description for the commit. Then press esc and type :wq  

5. Push your changes to Github  
```git push```

### Get updates from main branch to your own branch:
1. Check out main branch  
```git checkout main```

2. Pull updates from remote main branch to local main branch    
```git pull```

3. Change back to feature branch  
```git checkout <featurename>```

4. Merge main branch code to feature branch  
```git merge main```

### Once the feature is completed, merge it to main branch:  
You can create a pull request in Github to merge your branch to main branch  

#### OR
1. Checkout main branch  
```git checkout main```  

2. Merge your feature branch to main branch  
```git merge <featurename>```  

3. Delete local feature branch   
```git branch -d <featurename>```  

4. Delete remote feature branch  
```git push origin --delete <featurename>```  

**Note: Best practice to commit your changes regularly**