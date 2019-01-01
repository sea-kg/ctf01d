
// post request to server Async
function getAjax (url, callback) {
    callback = callback || function(){};
	var tmpXMLhttp = null;
	if (window.XMLHttpRequest) {
		// code for IE7+, Firefox, Chrome, Opera, Safari
		tmpXMLhttp = tmpXMLhttp || new window.XMLHttpRequest();
	};
	tmpXMLhttp.onreadystatechange=function() {
		if (tmpXMLhttp.readyState==4) {
			if (tmpXMLhttp.responseText == '') {
                obj = { 'result' : 'fail' };
                callback('fail', null);
            } else {
				try {
					var obj = JSON.parse(tmpXMLhttp.responseText);
                    callback(null, obj);
                    delete obj;
				} catch(e) {
					console.error(e.name + ':' + e.message + '\n stack:' + e.stack + '\n' + tmpXMLhttp.responseText);
				}
				delete tmpXMLhttp;
			}
		}
	}
	tmpXMLhttp.open('GET', url, true);
	tmpXMLhttp.send();
};
var scoreboard_content = document.getElementById('scoreboard_content');
var loader_content = document.getElementById('loader_content');

function updateUIValue(t, teamID, paramName){
    var newValue = '';
    if (paramName == 'score') {
        newValue = t[paramName].toFixed(2);
    } else {
        newValue = '' + t[paramName];
    }
    var el = document.getElementById(teamID + '_' + paramName);
    if (el) {
        var prevVal = el.innerHTML;
        if (prevVal != newValue) {
            document.getElementById(teamID + '_' + paramName).innerHTML = newValue;
        }
    } else {
        console.error('Not found element: ' + teamID + '_' + paramName);
    }
};

var inSwitch = false;

function switchUITeamRows(teamID1, teamID2){
    console.log('switchUITeamRows: ' + teamID1 + ' <-> ' + teamID2);
    if (inSwitch) {
    /*    setTimeout(function(teamID1, teamID2){
            switchUITeamRows(teamID1, teamID2);
        },50);*/
        return;
    }
    inSwitch = true;
    var el1 = document.getElementById(teamID1);
    var el2 = document.getElementById(teamID2);

    el1.style.transform = 'translateY(100px)';
    // el2.style.transform = 'translateY(-100px)';
    setTimeout(function(){
        el2.parentNode.insertBefore(el2, el1);
        el1.style.transform = '';
        el2.style.transform = '';
        inSwitch = false;
    },400);
}

function updateScoreboard() {
    getAjax('/api/v1/scoreboard', function(err, resp){
        if (err) {
            scoreboard_content.style.display = 'none';
            loader_content.style.display = 'block';
            return;
        }
        var teamIDs = [];
        for(var teamID in resp){
            var t = resp[teamID];
            teamIDs.push(teamID);
            updateUIValue(t, teamID, 'place');
            updateUIValue(t, teamID, 'score');

            for(var sService in t){
                if(sService.indexOf('service') == 0){
                    var newState = t[sService]['status'];
                    var newAttack = t[sService]['attack'];
                    var newDefence = t[sService]['defence'];
                    var newSLA = t[sService]['sla'];
                    var el = document.getElementById(teamID + '_' + sService);
                    if (!el.classList.contains(newState)) {
                        el.classList.remove('up');
                        el.classList.remove('down');
                        el.classList.remove('mumble');
                        el.classList.remove('corrupt');
                        el.classList.remove('shit');
                        el.classList.add(newState);
                    }
                    document.getElementById(teamID + '_' + sService + '_ad').innerHTML = newDefence + ' / ' + newAttack;
                    document.getElementById(teamID + '_' + sService + '_sla').innerHTML = 'SLA: ' + newSLA + '%';
                };
            }
        }

        // sort by places
        var elms2 = [];
        var elms = document.getElementsByClassName('tm');
        for(var i = 0; i < elms.length; i++){
            var el1 = elms[i];
            var place1 = parseInt(resp[el1.id]['place'], 10);
            elms2.push({
                e: elms[i],
                p: place1
            });
        }
        elms2.sort(function(a, b) {
            return a.p - b.p;
        });
        for(var i = 0; i < elms2.length; i++){
            elms2[i].e.style.top = ((i+1)*70) + 'px';
        }
        var bSorted = false;

        while (!bSorted){
            bSorted = true;
            
        }

        // open controls
        if (scoreboard_content.style.display != 'block') {
            scoreboard_content.style.display = 'block'
        }
        if (loader_content.style.display != 'none') {
            loader_content.style.display = 'none';
        }
    });
}

// init scoreboard
getAjax('/api/v1/game', function(err, resp){
    if (err) {
        console.error("Problem with game info ", err);
        return;
    }
    document.getElementById('game_name').innerHTML = resp.game_name;
    document.getElementById('game_time_range').innerHTML = resp.game_start + ' - ' + resp.game_end;
    
    console.log(resp);

    // generate teams-services table
    var sContent = 
    "<div class='scoreboard' id='table_scoreboard'>"
    + "    <div class='hdrs'>"
    + "        <div class='place'>#</div>"
    + "        <div class='team-logo'></div>"
    + "        <div class='team'>Team</div>"
    + "        <div class='score'>Score</div>";
    for (var i = 0; i < resp.services.length; i++) {
        sContent += "<div class='service'>" + resp.services[i].name + "<br><small>(service)</small></div>";
    }
    sContent += "  </div>";
    var sTeamListSelect = '';

    for (var iteam = 0; iteam < resp.teams.length; iteam++) {
        var sTeamId = resp.teams[iteam].id;
        sTeamListSelect += '<option value=' + sTeamId + '>' + sTeamId + '</option>';
        sContent += ""
            + "<div class='tm' id='" + sTeamId + "'>"
            + "  <div class='place' id='" + sTeamId + "_place' >" + (iteam + 1) + "</div>"
            + "  <div class='team-logo' id='" + sTeamId + "_logo' ><img class='team-logo' src='" + resp.teams[iteam].logo + "'/></div>"
            + "  <div class='team' id='" + sTeamId + "_name'>"
            + "    <div class='team-name'>" + resp.teams[iteam].name + "</div>"
            + "    <div class='team-ip'> id: " + sTeamId + ", ip: " + resp.teams[iteam].ip_address + "</div>"
            + "  </div>"
            + "  <div class='score' id='" + sTeamId + "_score'>0</div>";

        for (var i = 0; i < resp.services.length; i++) {
            sContent += ""
            + "<div class='service'>"
            + "  <div class='service-status down' id='" + sTeamId +  "_" + resp.services[i].id + "'> "
            + '   <div class="service-att-def">'
            + '       <div class="d-icn def" id="' + sTeamId +  '_' + resp.services[i].id + '_def">0</div>'
            + ' | '
            + '       <div class="d-icn att" id="' + sTeamId +  '_' + resp.services[i].id + '_att">0</div>'
            + '   </div>'
            + '   <div class="service-sla d-icn upt" id="' + sTeamId +  '_' + resp.services[i].id + '_sla">0.0%</div>'
            + "  </div>"
            + "</div>\n";
        }
        sContent += "</div>";
    }
    sContent += "</div>";

    document.getElementById('scoreboard_content').innerHTML = sContent;
    document.getElementById('team_list').innerHTML = sTeamListSelect;

    updateScoreboard();

    // start poling
    setInterval(function(){
        updateScoreboard()
    }, 3000);
});
