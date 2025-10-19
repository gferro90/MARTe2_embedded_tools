/**
 * @file FileManagerInterface.js 
 * @date 27/03/2019
 * @author Andre' Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 * Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.
 */
/**
 * Interface to the FileManagerInterface class. Creates one button for each Message.
 */
class FileManagerInterface extends MARTeObject {

    /**
     * NOOP
     */
    constructor() {
        super();
    }


    /**
     * NOOP
     */
    prepareDisplay(target) {
        this.target = target;
    }

    /**
     * Renders the data on the navigation tree.
     * 
     * @param {obj} jsonData the data as received by the server and which should contain a list of objects.
     */
    displayData(jsonData) {
        this.handleHeader();
        
        // handle the payload
        this.handlePayload(jsonData["Payload"]);
    }
    
    
    handleHeader(){
            var oldDiv=document.getElementById("MainDiv");
            if(oldDiv!=undefined){
                oldDiv.remove();
            }
        
            var div = document.createElement('div')
            div.setAttribute("id", "MainDiv");            

            var uploadBtn = document.createElement("button");
            uploadBtn.innerHTML = "Upload";

            div.appendChild(uploadBtn);
            this.target.appendChild(div);

            this.dialogDiv = document.createElement("div");       
            this.fileIn = document.createElement("input");
            this.fileIn.setAttribute("type", "file");
            this.upBtn = document.createElement("button");
            this.upBtn.innerHTML = "Upload";

            this.dialogDiv.appendChild(this.fileIn);
            this.dialogDiv.appendChild(this.upBtn);

            uploadBtn.onclick = function() {
                this.target.appendChild(this.dialogDiv);
            }.bind(this);
            
            this.upBtn.onclick = function() {
                var file_data = this.fileIn.files[0];
                var form_data = new FormData();
                form_data.append("file", file_data);
  
                var fullURL = MARTeLoader.instance().getDataUrl(this.getPath());
                if (fullURL.includes("?")) {
                    fullURL += ("&command=UPLOAD");
                }
                else {
                    fullURL += ("?command=UPLOAD");
                }
                fullURL+="&filePath=";
                fullURL+=file_data.name;

                var xhr = new XMLHttpRequest();
                xhr.onload = () => {
                    if (xhr.status >= 200 && xhr.status < 300) {
                            location.reload();
                    }
                }

                xhr.open('POST', fullURL, true);
                xhr.send(form_data);
            }.bind(this);

            
    }
    
    
    handlePayload(payloadData){
        var cnt=0;
        var table = document.createElement("table");
        var done = false;
        while(!done){
            var colName="Archive_";
            colName+=cnt;
            if(payloadData[colName]===undefined){
                done=true;
            }
            else{
                var row=table.insertRow(cnt);
                var col=row.insertCell(0);

                var fileName=payloadData[colName].split('/');
                var a = document.createElement('a');
                var linkText = document.createTextNode(fileName[fileName.length-1]);
                a.appendChild(linkText);
                a.title = fileName[fileName.length-1];
 
                var fullURL = "?";
                var myPath = MARTeLoader.instance().getDataUrl(this.getPath());
                var temp=myPath.split("?");
                myPath=temp[0]
                if (myPath.includes("/")) {
                    myPath=myPath.replace("/","");
                }
                fullURL+="ObjPath=";
                fullURL+=myPath;

                fullURL+="&command=DOWNLOAD";
                fullURL+="&TextMode=1";

                fullURL+="&filePath=";
                var myPath=payloadData["FilePath"];
                if(myPath=="."){
                    myPath="";
                }
                var baseDir=payloadData["BasePath"];
                
                var fullPath=baseDir+"/"+myPath;
                fullPath.replace("//","/");
                
                var elements=fullPath.split("/");
                var newElements=[];
                for(var n=0; n<elements.length; n++){
                    if(elements[n]==".."){
                        newElements.pop();
                    }
                    else{
                        newElements.push(elements[n]);
                    }
                }
                
                var baseElements=baseDir.split("/");

                var commonElements=0;
                for(var n=0; n<newElements.length; n++){
                    if(n>=baseElements.length){
                        break;
                    }
                    if(newElements[n]==baseElements[n]){
                        commonElements++;
                    }
                }
                
                //rebase to common elements
                myPath="";
                for(var n=0; n<(baseElements.length-commonElements); n++){
                    if(myPath!=""){
                        myPath+="/";
                    }
                    myPath+=".."
                }
                
                //add the new elements
                for(var n=commonElements; n<newElements.length; n++){
                    if(newElements[n]!=baseElements[n]){
                        if(myPath!=""){
                            myPath+="/";
                        }
                        myPath+=newElements[n];
                    }
                }
                
                
                if(myPath!=""){
                    myPath+="/";
                }
                myPath+=fileName[fileName.length-1];
                fullURL+=myPath;

                a.href = fullURL;
                col.appendChild(a);
                cnt++;
            }
        }
        
        var mainDiv=document.getElementById("MainDiv");
        mainDiv.appendChild(table);
        
    }
    
    
}

