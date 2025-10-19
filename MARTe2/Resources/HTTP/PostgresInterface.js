/**
 * @file PostgresInterface.js 
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
 * Interface to the PostgresInterface class. Creates one button for each Message.
 */
class PostgresInterface extends MARTeObject {

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
        var header=jsonData["Header"];
        this.handleHeader(header);
        
        var payload=jsonData["Payload"];
        // handle the payload
        this.handlePayload(payload);
    }
    
    handleHeader(headerData){
        if(headerData===undefined){
            var oldDiv=document.getElementById("MainDiv");
            if(oldDiv!=undefined){
                oldDiv.remove();
            }
        
            var div = document.createElement('div')
            div.setAttribute("id", "MainDiv");
            this.target.appendChild(div);            
        }
        else{
            var keys = Object.keys(headerData);
            if(keys[0]=="Table_List"){
                this.handleTableList(headerData);
            }
            else if(keys[0]=="Column_List"){
                this.handleColumnList(headerData);
            }
        }        
    }
    
    handleTableList(headerData){
            var oldDiv=document.getElementById("MainDiv");
            if(oldDiv!=undefined){
                oldDiv.remove();
            }
        
            var div = document.createElement('div')
            div.setAttribute("id", "MainDiv");            

            var headTable = document.createElement("table");
            headTable.setAttribute("border", "1");
            var tr = document.createElement("tr");
            var td1 = document.createElement("td");
        	
            var createOpt = document.createElement("button");
            createOpt.innerHTML = "CREATE OPTIONS";
            td1.appendChild(createOpt);

            var td2 = document.createElement("td");
            this.newTableName = document.createElement("input");
            this.newTableName.setAttribute("type", "text");
            this.newTableName.value="New Table Name"            
            td2.appendChild(this.newTableName);


            var td3 = document.createElement("td");
            this.numberOfOptions = document.createElement("input");
            this.numberOfOptions.setAttribute("type", "text");            
            this.numberOfOptions.value="New Table N Elements"            
            td3.appendChild(this.numberOfOptions);

            var td4 = document.createElement("td");
            var tableList = headerData["Table_List"];
            var nTables = Object.keys(tableList).length;
            this.tablesSelect=document.createElement("select");
            var option0 = document.createElement("option");
            option0.text = "Select Table";
            option0.value = "Select Table";
            this.tablesSelect.appendChild(option0);            
            for(var i=0; i<nTables; i++){
                var fieldTable="Table_";
                fieldTable+=i;
                var option = document.createElement("option");
                option.text = tableList[fieldTable];
                option.value = tableList[fieldTable];
                this.tablesSelect.appendChild(option);
            }
            td4.appendChild(this.tablesSelect);
            
            tr.appendChild(td1);
            tr.appendChild(td2);
            tr.appendChild(td3);
            tr.appendChild(td4);
            headTable.appendChild(tr);
            div.appendChild(headTable);
            this.target.appendChild(div);
            
            
            this.tablesSelect.onchange=function (e) {
                var fullURL = MARTeLoader.instance().getDataUrl(this.getPath());
                if (fullURL.includes("?")) {
                    fullURL += ("&command=COLUMNS");
                }
                else {
                    fullURL += ("?command=COLUMNS");
                }
                fullURL+="&table=";
                this.selectedTable=this.tablesSelect[this.tablesSelect.selectedIndex].value;
                fullURL+=this.selectedTable;                
            
                var xhttp = new XMLHttpRequest();
                var that = this;
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        try {
                            var jsonData = JSON.parse(this.responseText);
                            that.displayData(jsonData);
                        }
                        catch (e) {
                            console.log(e);
                        }
                    }
                };	
                xhttp.open("GET", fullURL, true);
                xhttp.send();   
            }.bind(this);            

            this.createBtn = document.createElement("button");
            this.createBtn.innerHTML = "CREATE TABLE";
            this.createBtn.onclick = function() {
                var fullURL = MARTeLoader.instance().getDataUrl(this.getPath());
                if (fullURL.includes("?")) {
                    fullURL += ("&command=CREATE");
                }
                else {
                    fullURL += ("?command=CREATE");
                }
                fullURL+="&table=";
                fullURL+=this.newTableName.value;                
                for(var i=0; i<this.numberOfOptions.value; i++){
                    var fieldName="Column_";
                    fieldName+=i;
                    fieldName+="=";
                    fieldName+=this.colName[i].value;
                    fieldName+=" ";
                    fieldName+=this.colTypeSel[i][this.colTypeSel[i].selectedIndex].value;
                    if(this.colNumElems[i].value != ""){
                        if(!isNaN(parseInt(this.colNumElems[i].value))){
                            fieldName+="(";
                            fieldName+=this.colNumElems[i].value;
                            fieldName+=")";
                        }
                    }
                    if(this.notNull[i].checked){
                        fieldName+=" NOT NULL"
                    }
                    fullURL+="&";
                    fullURL+=fieldName;
                }
                var xhttp = new XMLHttpRequest();
                var that = this;
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        try {
                            var jsonData = JSON.parse(this.responseText);
                            that.displayData(jsonData);
                        }
                        catch (e) {
                            console.log(e);
                        }
                    }
                };	
                xhttp.open("GET", fullURL, true);
                xhttp.send();                
	        }.bind(this);

            
            createOpt.onclick = function() {
                var table = document.createElement("table");
                table.setAttribute("border", "1");
                var newColNRows=parseInt(this.numberOfOptions.value);
                this.colName=[];
                this.colNumElems=[];
                this.colTypeSel=[];
                this.notNull=[];
                if(!isNaN(newColNRows)){
                for(var i=0; i<newColNRows; i++){
                    var row=table.insertRow(i);
                    var name=row.insertCell(0);
                    var type=row.insertCell(1);
                    var nElems=row.insertCell(2);
                    var notNull=row.insertCell(3);

                    this.colName.push(document.createElement("input"));
                    this.colName[i].setAttribute("type", "text");
                    this.colName[i].value = "Column Name"
                    name.appendChild(this.colName[i]);            

                    this.colNumElems.push(document.createElement("input"));
                    this.colNumElems[i].setAttribute("type", "text");
                    this.colName[i].value = "Column N Elements"
                    nElems.appendChild(this.colNumElems[i]);            

                    this.colTypeSel.push(document.createElement("select"));
                    var optionsList=["int8", "serial8", "bit", "varbit", "bool", "box", "bytea", "char", "varchar", "cidr", "circle", "date", "float8", "inet", "int4", "json", "jsonb", "line", "lseg", "macaddr", "money", "path", "pg_lsn", "point", "polygon", "float4", "int2", "serial2", "serial4", "text", "time", "timetz", "tsquery", "tsvector", "txid_snapshot", "uuid", "xml"];
                    
                    for(var j=0; j<optionsList.length; j++){
                        var option = document.createElement("option");
                        option.text = optionsList[j];
                        option.value = optionsList[j];
                        this.colTypeSel[i].appendChild(option);
                    }
                    type.appendChild(this.colTypeSel[i]);

                    this.notNull.push(document.createElement("input"));
                    this.notNull[i].setAttribute("type", "checkbox");

                    var label = document.createElement("Label");
                    label.innerHTML="Not Null";

                    notNull.appendChild(this.notNull[i]);
                    notNull.appendChild(label);
                }
                }
                var mainDiv=document.getElementById("MainDiv");
                mainDiv.appendChild(table);
                mainDiv.appendChild(this.createBtn);
                
            }.bind(this);

    }
    
    handleColumnList(headerData){
            var oldDiv=document.getElementById("MainDiv");
            if(oldDiv!=undefined){
                oldDiv.remove();
            }
        
            var div = document.createElement('div')
            div.setAttribute("id", "MainDiv");
            

            var tableAdd = document.createElement("table");
            tableAdd.setAttribute("border", "1");

            var colList=headerData["Column_List"];
            var keys = Object.keys(colList);
            this.numberOfCols=keys.length;
            this.colName=[];
            for (var i=0; i<this.numberOfCols; i++){
                var row=tableAdd.insertRow(i);
                var cell=row.insertCell(0);
                
                var column=document.createElement("input");
                var colId="Column_";
                colId+=i;
                column.setAttribute("id", colId);
                column.setAttribute("type", "text");
                column.value = colList[colId]+" Value";
                this.colName.push(colList[colId]);
                cell.appendChild(column);     
            }

            var addBtn = document.createElement("button");
            addBtn.innerHTML = "ADD RECORD";
            addBtn.onclick = function() {
                var fullURL = MARTeLoader.instance().getDataUrl(this.getPath());
                if (fullURL.includes("?")) {
                    fullURL += ("&command=INSERT");
                }
                else {
                    fullURL += ("?command=INSERT");
                }
                fullURL+="&table=";
                fullURL+=this.selectedTable;                

                for(var i=0; i<this.numberOfCols; i++){
                    var colId="Column_"+i;
                    var cell=document.getElementById(colId);

                    if((cell.value != (this.colName[i]+" Value")) && (cell.value != "") && (cell.value != "NULL")){
                        var fieldName="Column_";
                        fieldName+=this.colName[i];
                        fieldName+="=";
                        fieldName+="'";
                        fieldName+=cell.value;
                        fieldName+="'";
                        fullURL+="&";
                        fullURL+=fieldName;
                    }
                }
                var xhttp = new XMLHttpRequest();
                var that = this;
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        try {
                            var jsonData = JSON.parse(this.responseText);
                            that.displayData(jsonData);
                        }
                        catch (e) {
                            console.log(e);
                        }
                    }
                };	
                xhttp.open("GET", fullURL, true);
                xhttp.send();                
	        }.bind(this);
            
            div.appendChild(tableAdd);
            div.appendChild(addBtn);


            var tableRem = document.createElement("table");
            tableRem.setAttribute("id", "tableRemRecord");
            tableRem.setAttribute("border", "1");

            this.addCondition(tableRem);

            var selColSel = document.createElement("select");
            var emptyOption1 = document.createElement("option");
            emptyOption1.text = "Select Table";
            emptyOption1.value = "Select Table";
            selColSel.appendChild(emptyOption1);            

            for (var i=0; i<this.numberOfCols; i++){
                var option = document.createElement("option");
                option.text = this.colName[i];
                option.value = this.colName[i];
                selColSel.appendChild(option);            
            }


            var tableSel = document.createElement("table");
            tableSel.setAttribute("id", "tableSelRecord");
            tableSel.setAttribute("border", "1");

            var row=tableSel.insertRow(0);
            var sortByCell=row.insertCell(0);
            var sortPolCell=row.insertCell(1);
            var limCell=row.insertCell(2);
            
            var sortBySelect=document.createElement("select");
            sortBySelect.setAttribute("id", "sortBySelect");

            var emptyOption = document.createElement("option");
            emptyOption.text = "Select Table";
            emptyOption.value = "Select Table";
            sortBySelect.appendChild(emptyOption);            

            for (var i=0; i<this.numberOfCols; i++){
                var option = document.createElement("option");
                option.text = this.colName[i];
                option.value = this.colName[i];
                sortBySelect.appendChild(option);            
            }
            sortByCell.appendChild(sortBySelect);

            var sortPolSelect=document.createElement("select");
            sortPolSelect.setAttribute("id", "sortPolSelect");
            
            var optionDesc = document.createElement("option");
            optionDesc.text = "ASC";
            optionDesc.value = "ASC";
            sortPolSelect.appendChild(optionDesc);            

            var optionAsc = document.createElement("option");
            optionAsc.text = "DESC";
            optionAsc.value = "DESC";
            sortPolSelect.appendChild(optionAsc);            
            sortPolCell.appendChild(sortPolSelect);
            
            var limInput=document.createElement("input");
            var limInputId="limInput";
            limInput.setAttribute("id", limInputId);

            limInput.setAttribute("type", "text");            
            limInput.value="Limit"
            limCell.appendChild(limInput);
	            


            var selBtn = document.createElement("button");
            selBtn.innerHTML = "SELECT";
            selBtn.onclick = function() {
                var fullURL = MARTeLoader.instance().getDataUrl(this.getPath());
                if (fullURL.includes("?")) {
                    fullURL += ("&command=SELECT");
                }
                else {
                    fullURL += ("?command=SELECT");
                }
                fullURL+="&table=";
                fullURL+=this.selectedTable;                

                if(selColSel.selectedIndex>0){
                    fullURL+="&col=";
                    fullURL+=selColSel[selColSel.selectedIndex].value;                
                }


                var tableRem=document.getElementById("tableRemRecord");
                var nRows=tableRem.rows.length;
                var condition="";
                for(var i=0; i<nRows; i++){
                    var colSelId="colSelectId_"+i;
                    var colSel=document.getElementById(colSelId);
                    
                    var compSelId="compSelectId_"+i;
                    var compSel=document.getElementById(compSelId);
                    
                    var condInId="condInputId_"+i;
                    var condIn=document.getElementById(condInId);


                    var concSelId="concSelectId_"+i;
                    var concSel=document.getElementById(concSelId);

                    if(condIn.value!=""){
                        condition+=colSel[colSel.selectedIndex].value;
                        condition+=compSel[compSel.selectedIndex].value;
                        condition+="'";
                        condition+=condIn.value;
                        condition+="' ";
                        condition+=concSel[concSel.selectedIndex].value;
                        condition+=" ";
                    }                                        

                }
               
                if(condition != ""){
                    fullURL+="&condition=";
                    fullURL+=condition;
                }

                var sortBySel=document.getElementById("sortBySelect");
                var sortPolSel=document.getElementById("sortPolSelect");
                if(sortBySel.selectedIndex>0){
                    fullURL+="&orderBy=";
                    fullURL+=sortBySel[sortBySel.selectedIndex].value;
                    fullURL+="&sortPolicy=";
                    fullURL+=sortPolSel[sortPolSel.selectedIndex].value;
                }

                var limIn=document.getElementById("limInput");
                if(limIn.value!="" && limIn.value!="Limit" && (!isNaN(parseInt(limIn.value)))){
                    fullURL+="&limit=";
                    fullURL+=limIn.value;
                }
                
                var xhttp = new XMLHttpRequest();
                var that = this;
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        try {
                            var jsonData = JSON.parse(this.responseText);
                            that.displayData(jsonData);
                        }
                        catch (e) {
                            console.log(e);
                        }
                    }
                };	
                xhttp.open("GET", fullURL, true);
                xhttp.send();                
	        }.bind(this);
            
            
            var remBtn = document.createElement("button");
            remBtn.innerHTML = "DEL RECORD";
            remBtn.onclick = function() {
                var fullURL = MARTeLoader.instance().getDataUrl(this.getPath());
                if (fullURL.includes("?")) {
                    fullURL += ("&command=DELETE");
                }
                else {
                    fullURL += ("?command=DELETE");
                }
                fullURL+="&table=";
                fullURL+=this.selectedTable;                
                var tableRem=document.getElementById("tableRemRecord");
                var nRows=tableRem.rows.length;
                var condition="";
                for(var i=0; i<nRows; i++){
                    var colSelId="colSelectId_"+i;
                    var colSel=document.getElementById(colSelId);
                    
                    var compSelId="compSelectId_"+i;
                    var compSel=document.getElementById(compSelId);
                    
                    var condInId="condInputId_"+i;
                    var condIn=document.getElementById(condInId);
                    
                    var concSelId="concSelectId_"+i;
                    var concSel=document.getElementById(concSelId);
                    if(condIn.value!=""){
                        condition+=colSel[colSel.selectedIndex].value;
                        condition+=compSel[compSel.selectedIndex].value;
                        condition+="'";
                        condition+=condIn.value;
                        condition+="'";
                        condition+=concSel[concSel.selectedIndex].value;
                    }                                        
                }
                if(condition != ""){
                    fullURL+="&condition=";
                    fullURL+=condition;
                }
                var xhttp = new XMLHttpRequest();
                var that = this;
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        try {
                            var jsonData = JSON.parse(this.responseText);
                            that.displayData(jsonData);
                        }
                        catch (e) {
                            console.log(e);
                        }
                    }
                };	
                xhttp.open("GET", fullURL, true);
                xhttp.send();                
	        }.bind(this);

            var deleteBtn = document.createElement("button");
            deleteBtn.innerHTML = "DEL TABLE";

            deleteBtn.onclick = function() {
                var fullURL = MARTeLoader.instance().getDataUrl(this.getPath());
                if (fullURL.includes("?")) {
                    fullURL += ("&command=DESTROY");
                }
                else {
                    fullURL += ("?command=DESTROY");
                }
                fullURL+="&table=";
                fullURL+=this.selectedTable;                
                var xhttp = new XMLHttpRequest();
                var that = this;
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        try {
                            var jsonData = JSON.parse(this.responseText);
                            that.displayData(jsonData);
                        }
                        catch (e) {
                            console.log(e);
                        }
                    }
                };	
                xhttp.open("GET", fullURL, true);
                xhttp.send();                
	        }.bind(this);

            
            var superTable = document.createElement("table");
            
            var tr1 = document.createElement("tr");
            var td1 = document.createElement("td");

            var selColLabel = document.createElement("Label");
            selColLabel.innerHTML="Select  ";

            td1.appendChild(selColLabel);
            td1.appendChild(selColSel);
            tr1.appendChild(td1);
            superTable.appendChild(tr1);

            var tr2 = document.createElement("tr");
            var td2 = document.createElement("td");
            
            var sortByLabel = document.createElement("Label");
            sortByLabel.innerHTML="Sort By";

            td2.appendChild(sortByLabel);
            td2.appendChild(tableSel);
            tr2.appendChild(td2);
            superTable.appendChild(tr2);

            var tr3 = document.createElement("tr");
            var td3 = document.createElement("td");

            var whereLabel = document.createElement("Label");
            whereLabel.innerHTML="Where";

            td3.appendChild(whereLabel);
            td3.appendChild(tableRem);
            tr3.appendChild(td3);
            superTable.appendChild(tr3);


            var tr4 = document.createElement("tr");
            var td4 = document.createElement("td");

            selBtn.style.width = '100px';
            td4.appendChild(selBtn);
            tr4.appendChild(td4);
            superTable.appendChild(tr4);

            var tr5 = document.createElement("tr");
            var td5 = document.createElement("td");

            remBtn.style.width = '100px';
            td5.appendChild(remBtn);
            tr5.appendChild(td5);
            superTable.appendChild(tr5);

            var tr6 = document.createElement("tr");
            var td6 = document.createElement("td");

            deleteBtn.style.width = '100px';
            td6.appendChild(deleteBtn);
            tr6.appendChild(td6);
            superTable.appendChild(tr6);

            div.appendChild(superTable);
            
            this.target.appendChild(div);
    }    
    
    
    handlePayload(payloadData){
        var cnt=0;
        var table = document.createElement("table");
        table.setAttribute("border", "1");
        var done = false;
        while(!done){
            var colName="Column_";
            colName+=cnt;
            if(payloadData[colName]===undefined){
                done=true;
            }
            else{
                var row=table.rows[0];
                if(row===undefined){
                    var row=table.insertRow(0);
                }
                var col=row.insertCell(cnt);
                var content = document.createTextNode(payloadData[colName]);
                var span = document.createElement('span');
                span.style.fontWeight = 'bold';
                
                span.appendChild(content);
                col.appendChild(span);
                
                cnt++;
            }
        }
        
        var doneRow=false;
        var i=0;
        while(!doneRow){
            var doneCol=false;
            var j=0;
            while(!doneCol){
                var recName="Record_";
                recName+=i;
                recName+="_";
                recName+=j;
                if(payloadData[recName]===undefined){
                    doneCol=true;
                    if(j==0){
                        doneRow=true;
                    }
                }
                else{
                    var row=table.rows[i+1];
                    if(row===undefined){
                        row=table.insertRow(i+1);
                    }
                    var col=row.insertCell(j);
                    if(payloadData[recName].startsWith("http:") || payloadData[recName].startsWith("https:")){
                        var a = document.createElement('a');
                        var linkText = document.createTextNode(payloadData[recName]);
                        a.appendChild(linkText);
                        a.title = payloadData[recName];
                        a.href = payloadData[recName];
                        col.appendChild(a);
                    }
                    else{
                        var content = document.createTextNode(payloadData[recName]);
                        col.appendChild(content);
                    }
                    j++;
                }
            }
            i++;
        }
        var mainDiv=document.getElementById("MainDiv");
        mainDiv.appendChild(table);
        
    }
    
    addCondition(tableRem){
            var row=tableRem.insertRow(-1);
            var colCell=row.insertCell(0);
            var compCell=row.insertCell(1);
            var condCell=row.insertCell(2);
            var concCell=row.insertCell(3);
            
            var colSelect=document.createElement("select");
            var rowN=tableRem.rows.length-1;
            var colSelectId="colSelectId_"+rowN;
            colSelect.setAttribute("id", colSelectId);
            for (var i=0; i<this.numberOfCols; i++){
                var option = document.createElement("option");
                option.text = this.colName[i];
                option.value = this.colName[i];
                colSelect.appendChild(option);            
            }
            colCell.appendChild(colSelect);
            
            var compList=["=", ">", "<", "<=", ">=", "!="];
            var compSelect=document.createElement("select");
            var compSelectId="compSelectId_"+rowN
            compSelect.setAttribute("id", compSelectId);

            for (var i=0; i<compList.length; i++){
                var option = document.createElement("option");
                option.text = compList[i];
                option.value = compList[i];
                compSelect.appendChild(option);            
            }
            compCell.appendChild(compSelect);
            
            var condInput=document.createElement("input");
            var condInputId="condInputId_"+rowN;
            condInput.setAttribute("id", condInputId);

            condInput.setAttribute("type", "text");   
            condCell.appendChild(condInput);
            
            if(this.concSelect!=undefined){
                this.concSelect.removeEventListener("change", this.concSelect.onchange);
            }

            this.concSelect=document.createElement("select");
            var concSelectId="concSelectId_"+rowN;
            this.concSelect.setAttribute("id", concSelectId);

            var emptyOption = document.createElement("option");
            emptyOption.text = "";
            emptyOption.value = "";
            this.concSelect.appendChild(emptyOption);            

            var andOption = document.createElement("option");
            andOption.text = "AND";
            andOption.value = "AND";
            this.concSelect.appendChild(andOption);            

            var orOption = document.createElement("option");
            orOption.text = "OR";
            orOption.value = "OR";
            this.concSelect.appendChild(orOption);            
            concCell.appendChild(this.concSelect);    
            
            this.concSelect.onchange=function (e) {
                var tableRem=document.getElementById("tableRemRecord");
                if(this.concSelect.selectedIndex>0){
                    this.addCondition(tableRem);
                }
            }.bind(this);               
    }
    
}

