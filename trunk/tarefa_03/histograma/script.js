function makeGraph()
{
	var container = document.getElementById("graph");
	var labels = document.getElementById("labels");
	var dnl = container.getElementsByTagName("li");
	var lastyear = 0;
	var max = 0;
	
	for(var i = 0; i < dnl.length; i++)
	{
		var item = dnl.item(i);
		var value = item.innerHTML;
		var content = value.split(":");
		var color =item.style.background;
		value = content[0];
		if(parseInt(max) < parseInt(value))
		{
			max = value;
		}
	}
	
	for(var i = 0; i < dnl.length; i++)
	{
		var item = dnl.item(i);
		var value = item.innerHTML;
		var content = value.split(":");
		var color =item.style.background;
		value = content[0];
		value_rel = 200*value/max
		item.style.height= value_rel + "px";
		item.style.top=(199 - value_rel) + "px";
		item.style.left = (i * 50 + 20) + "px";
		item.style.height = value_rel + "px";
		item.innerHTML = value;
		item.style.visibility="visible";
		if(content.length > 2)
		{
			color = content[2];
			item.style.background=color;
		}		
		left = new String(i * 50 + 58) + "px";
		
		labels.innerHTML = labels.innerHTML + "<span style='position:absolute;top:-16px;left:"+ left+";background:"+ color+"'>" + content[1] + "</span>";
		
	}
	
	for (var i=3; i>=0; i--)
	{
		document.getElementById("scale").innerHTML += new String(parseInt(i*max/3));
		document.getElementById("scale").innerHTML += "<br \\><br \\><br \\>";
	}
}

window.onload=makeGraph;
