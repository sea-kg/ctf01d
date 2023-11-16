
// init tabs
var tabs = document.getElementsByClassName('ctf01d-tab');
var tabs_content = document.getElementsByClassName('ctf01d-tab-content');
function switchToTabContent() {
    var tabcontentid = this.getAttribute('tabcontentid');
    for (var i = 0; i < tabs.length; i++) {
        tabs[i].classList.remove('active');
    }
    for (var i = 0; i < tabs_content.length; i++) {
        tabs_content[i].style.display = '';
    }
    this.classList.add('active');
    document.getElementById(tabcontentid).style.display = 'block';
}
for (var i in tabs) {
    // console.log(tabs[i]);
    tabs[i].onclick = switchToTabContent;
}

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
                // obj = { 'result' : 'fail' };
                callback('fail', null);
            } else {
				try {
					var obj = JSON.parse(tmpXMLhttp.responseText);
                    callback(null, obj);
                    // delete obj;
				} catch(e) {
					console.error(e.name + ':' + e.message + '\n stack:' + e.stack + '\n' + tmpXMLhttp.responseText);
				}
				// delete tmpXMLhttp;
			}
		}
	}
	tmpXMLhttp.open('GET', url, true);
	tmpXMLhttp.send();
};
var scoreboard_content = document.getElementById('scoreboard_content');
var loader_content = document.getElementById('loader_content');

function _animateElement(el, enable) {
    if (el == null) {
        console.error("_animateElement el is null");
        return;
    }
    el.style.animation = enable ? "blinking 0.8s reverse infinite" : '';
}

function _animateElementOneTime(el) {
    if (el == null) {
        console.error("_animateElementOneTime el is null");
        return;
    }
    el.style.animation = "fastblinking 0.8s reverse infinite";
    var i = setTimeout(function() {
        el.style.animation = '';
        clearTimeout(i);
    },800);
}

function silentUpdate(elid, newValue) {
    var el = document.getElementById(elid)
    if (!el) {
        console.error("Not found element with id " + elid);
        return;
    }
    if (el.innerHTML != newValue) {
        el.innerHTML = newValue;
        _animateElementOneTime(el);
        // TODO make simple anim
    }
}

function silentUpdateWithoutAnimation(elid, newValue) {
    var el = document.getElementById(elid)
    if (!el) {
        console.error("Not found element with id " + elid);
        return;
    }
    if (el.innerHTML != newValue) {
        el.innerHTML = newValue;
        // _animateElementOneTime(el);
        // TODO make simple anim
    }
}

var g_counterShowMassAction = 0;

function showActionAutomatization() {
    g_counterShowMassAction++;
    var el_mass_action = document.getElementById('current_mass_action');
    el_mass_action.style.display = 'block';
    el_mass_action.classList.remove("mass-action-firstblood")
    el_mass_action.classList.add("mass-action-automatization")
    var i = setTimeout(function() {
        g_counterShowMassAction--;
        if (g_counterShowMassAction <= 0) {
            g_counterShowMassAction = 0;
            el_mass_action.style.display = '';
        }
        clearTimeout(i);
    },5000);
}

function showActionFirstblood() {
    g_counterShowMassAction++;
    var el_mass_action = document.getElementById('current_mass_action');
    el_mass_action.classList.remove("mass-action-automatization")
    el_mass_action.classList.add("mass-action-firstblood")
    el_mass_action.style.display = 'block';
    var i = setTimeout(function() {
        g_counterShowMassAction--;
        if (g_counterShowMassAction <= 0) {
            g_counterShowMassAction = 0;
            el_mass_action.style.display = '';
        }
        clearTimeout(i);
    },5000);
}

function updateUIValue(t, teamID, paramName){
    var newValue = '';
    if (paramName == 'points') {
        newValue = t[paramName].toFixed(1);
    } else {
        newValue = '' + t[paramName];
    }
    var elem_id = paramName + '-' + teamID;
    var el = document.getElementById(elem_id);
    if (el) {
        var prevVal = el.innerHTML;
        if (prevVal != newValue) {
            if (paramName == "tries") {
                if (prevVal != "") {
                    var diff = parseInt(newValue, 10) - parseInt(prevVal, 10);
                    // console.log("diff", diff)
                    if (diff >= 5) {
                        showActionAutomatization();
                    }
                    _animateElement(document.getElementById('tries-icon-' + teamID), true);
                    if (diff != 0) {
                        newValue += " +" + diff;
                    }
                }
            }
            document.getElementById(elem_id).innerHTML = newValue;
        } else {
            if (paramName == "tries") {
                _animateElement(document.getElementById('tries-icon-' + teamID), false);
            }
        }
        if (paramName == "place" && prevVal != newValue) {
            if (newValue == "1") {
                el.classList.remove('place-2st');
                el.classList.remove('place-3st');
                if (!el.classList.contains('place-1st')) {
                    el.classList.add('place-1st');
                }
            } else if (newValue == "2") {
                el.classList.remove('place-1st');
                el.classList.remove('place-3st');
                if (!el.classList.contains('place-2st')) {
                    el.classList.add('place-2st');
                }
            } else if (newValue == "3") {
                el.classList.remove('place-1st');
                el.classList.remove('place-2st');
                if (!el.classList.contains('place-3st')) {
                    el.classList.add('place-3st');
                }
            } else {
                el.classList.remove('place-1st');
                el.classList.remove('place-2st');
                el.classList.remove('place-3st');
            }
        }
    } else {
        console.error('Not found element: ' + elem_id);
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
        // console.log(resp);
        for (var serviceId in resp.s_sta) {
            var s = resp.s_sta[serviceId]
            silentUpdate(serviceId + '-first-blood', s.first_blood);
            silentUpdate(serviceId + '-all-flags-att', s.af_att)
            silentUpdate(serviceId + '-all-flags-def', s.af_def)
        }

        // game time
        var game_len_time = resp.game.t3 - resp.game.t0;
        var game_passed_time = resp.game.tc - resp.game.t0;
        // console.log("game_len_time", game_len_time);
        if (resp.game.tc < resp.game.t0) {
            silentUpdateWithoutAnimation(
                'game_current_time',
                'game started after: ' + (resp.game.t0 - resp.game.tc) + ' seconds'
            );
            document.getElementById('game_progress_time').style.display = 'none';
        } else if (resp.game.tc >= resp.game.t1 && resp.game.tc <= resp.game.t2) { // coffee break
            silentUpdateWithoutAnimation(
                'game_current_time',
                'the game will continue after the coffee break in ' + (resp.game.t2 - resp.game.tc) + ' seconds'
            );
        } else if (resp.game.tc > resp.game.t3) {
            silentUpdateWithoutAnimation('game_current_time', 'game ended');
            document.getElementById('game_progress_time').style.display = 'block';
            document.getElementById('game_progress_time').style.width = '100%';
        } else if (
            resp.game.t1 > resp.game.t0 && resp.game.t1 < resp.game.t3
            && resp.game.t2 > resp.game.t0 && resp.game.t2 < resp.game.t3
        ) { // coffee break enabled
            // console.log("game passed_time", (game_passed_time / game_len_time)*100);
            document.getElementById('game_progress_time').style.display = 'block';
            document.getElementById('game_progress_time').style.width = Math.ceil((game_passed_time / game_len_time)*100) + '%';
            if (resp.game.tc > resp.game.t0 && resp.game.tc < resp.game.t1) { // before coffee break
                silentUpdateWithoutAnimation(
                    'game_current_time',
                    'game time: ' + (resp.game.tc - resp.game.t0) + ' seconds and coffee break will start in ' + (resp.game.t1 - resp.game.tc) + ' seconds'
                );
                document.getElementById('game_progress_time').style.display = 'block';
            } else if (resp.game.tc > resp.game.t2 && resp.game.tc < resp.game.t3) { // after coffee break
                silentUpdateWithoutAnimation(
                    'game_current_time',
                    'game time: ' + (resp.game.tc - resp.game.t0) + ' seconds and game will end in ' + (resp.game.t3 - resp.game.tc) + ' seconds'
                );
            }
        } else if (resp.game.tc > resp.game.t0 && resp.game.tc < resp.game.t3) { // before coffe break
            silentUpdateWithoutAnimation('game_current_time', 'game time: ' + (resp.game.tc - resp.game.t0) + ' seconds, and game will end in ' + (resp.game.t3 - resp.game.tc) + ' seconds');
            document.getElementById('game_progress_time').style.display = 'block';
            document.getElementById('game_progress_time').style.width = Math.ceil((game_passed_time / game_len_time)*100) + '%';
        }

        var teamIDs = [];
        for(var teamID in resp.scoreboard){
            var t = resp.scoreboard[teamID];
            teamIDs.push(teamID);
            var elPointsTrend = document.getElementById(teamID + '-points-trend');
            var prevPoints = parseFloat(document.getElementById(teamID + '-points').innerHTML);
            var newPoints = parseFloat(t.points.toFixed(1));
            if (elPointsTrend.innerHTML == "??") {
                elPointsTrend.classList.add("trend-middle")
                elPointsTrend.classList.remove("trend-up")
                elPointsTrend.classList.remove("trend-down")
                elPointsTrend.innerHTML = "+0";
            } else {
                if (newPoints == prevPoints) {
                    elPointsTrend.classList.add("trend-middle")
                    elPointsTrend.classList.remove("trend-up")
                    elPointsTrend.classList.remove("trend-down")
                    elPointsTrend.innerHTML = "+0";
                } else if (newPoints > prevPoints) {
                    elPointsTrend.classList.remove("trend-middle")
                    elPointsTrend.classList.add("trend-up")
                    elPointsTrend.classList.remove("trend-down")
                    elPointsTrend.innerHTML = "+" + (newPoints - prevPoints).toFixed(1);
                } else {
                    elPointsTrend.classList.remove("trend-middle")
                    elPointsTrend.classList.remove("trend-up")
                    elPointsTrend.classList.add("trend-down")
                    elPointsTrend.innerHTML = "-" + (prevPoints - newPoints).toFixed(1);
                }
            }
            silentUpdate(teamID + '-points', newPoints.toFixed(1));

            updateUIValue(t, teamID, 'place');
            // updateUIValue(t, teamID, 'points');
            updateUIValue(t, teamID, 'tries');
            for(var sService in t.ts_sta){
                var newState = t.ts_sta[sService]['status'];
                var newAttackFlags = t.ts_sta[sService]['att'];
                // var newDefenceFlags = t.ts_sta[sService]['def'];
                var newAttackPoints = t.ts_sta[sService]['pt_att'];
                var newDefencePoints = t.ts_sta[sService]['pt_def'];
                var newSLA = t.ts_sta[sService]['sla'];
                var elId = 'status-' + teamID + '-' + sService;
                var el = document.getElementById(elId);
                if (el != null) {
                    if (!el.classList.contains(newState)) {
                        el.classList.remove('up');
                        el.classList.remove('down');
                        el.classList.remove('mumble');
                        el.classList.remove('corrupt');
                        el.classList.remove('shit');
                        el.classList.remove('wait');
                        el.classList.remove('coffeebreak');
                        el.classList.add(newState);
                        _animateElementOneTime(el);

                    }
                } else {
                    console.error(elId + '- not found');
                }
                var sCell = teamID + '-' + sService;
                // console.log(sCell);
                silentUpdate('att-' + sCell, newAttackFlags)
                // silentUpdate('def-' + sCell, newDefenceFlags)
                silentUpdate('pt_att-' + sCell, newAttackPoints)
                silentUpdate('pt_def-' + sCell, newDefencePoints)
                silentUpdate('sla-' + sCell, newSLA + "%")
            }
        }

        // sort by places
        var elms2 = [];
        var elms = document.getElementsByClassName('tm');
        for(var i = 0; i < elms.length; i++){
            var el1 = elms[i];
            var place1 = parseInt(resp["scoreboard"][el1.id]['place'], 10);
            elms2.push({
                e: elms[i],
                p: place1
            });
        }
        elms2.sort(function(a, b) {
            return a.p - b.p;
        });
        for(var i = 0; i < elms2.length; i++){
            elms2[i].e.style.top = (60 + (i+1)*60) + 'px';
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

function formatGameTimings(periods) {
    // TODO beauty print periods
}

// init scoreboard
getAjax('/api/v1/game', function(err, resp){
    if (err) {
        console.error("Problem with game info ", err);
        return;
    }
    document.getElementById('game_name').innerHTML = resp.game_name;

    // TODO beauty print periods 
    if (resp.game_has_coffee_break) {
        document.getElementById('game_time_range').innerHTML = 
            resp.game_start + ' - ' + resp.game_coffee_break_start + ' (coffee break) '
            + resp.game_coffee_break_end + ' - ' + resp.game_end;
    } else {
        document.getElementById('game_time_range').innerHTML = resp.game_start + ' - ' + resp.game_end;
    }

    // console.log(resp);

    // generate teams-services table
    var sContent = ""
        + "<div class='scoreboard' id='table_scoreboard'>"
        + "    <div class='hdrs'>"
        + "        <div class='place'>#</div>"
        + "        <div class='team-logo'></div>"
        + "        <div class='team'>Team</div>"
        + "        <div class='score'><div class='hdr-text'>points</div></div>";
    for (var i = 0; i < resp.services.length; i++) {
        var serviceId = resp.services[i].id;
        sContent += ''
        + '<div class="service"><b>' + resp.services[i].name + '</b><br>'
        + '  <div class="service-att-def">'
        + '      <div class="service-att-def-row">'
        + '          <div class="service-att-def-cell defence-flags" id="' + serviceId + '-all-flags-def">0.0</div>'
        + '          <div class="service-att-def-cell stollen-flags" id="' + serviceId + '-all-flags-att">0.0</div>'
        + '      </div>'
        + '      <div class="service-att-def-row">'
        + '          <div class="service-att-def-cell first-blood" id="' + serviceId +  '-first-blood">-</div>'
        + '          <div class="service-att-def-cell round-time">' + resp.services[i].round_time_in_sec + 's</div>'
        + '      </div>'
        + '  </div>'
        + "</div>";
    }
    sContent += ''
        + '        <div class="activity">Activity</div>'
        + '  </div>'
        + "  <div class='hdrs-time'>"
        + "    <div class='hdrs-time-fill' id='game_progress_time'></div>"
        + "    <div class='hdrs-time-game-current-time' id='game_current_time'>0</div>"
        + "  </div>";

    var sTeamListSelect = '';

    for (var iteam = 0; iteam < resp.teams.length; iteam++) {
        var sTeamId = resp.teams[iteam].id;
        sTeamListSelect += '<option value=' + sTeamId + '>' + sTeamId + '</option>';
        sContent += ""
            + "<div class='tm' id='" + sTeamId + "'>"
            + '  <div class="place" id="place-' + sTeamId + '" >?</div>'
            + "  <div class='team-logo'><img class='team-logo' src='" + resp.teams[iteam].logo + "'/></div>"
            + "  <div class='team'>"
            + "    <div class='team-name'>" + resp.teams[iteam].name + "</div>"
            + "    <div class='team-ip'> id: " + sTeamId + ", ip: " + resp.teams[iteam].ip_address + "</div>"
            + "  </div>"
            + '  <div class="score">'
            + '     <div class="points-sum" id="' + sTeamId + '-points">0</div>'
            + '     <div class="points-trend trend-down" id="' + sTeamId + '-points-trend">??</div>'
            + '  </div>';

        for (var i = 0; i < resp.services.length; i++) {
            var sServiceID = resp.services[i].id;
            sContent += ""
            + "<div class='service'>"
            + "  <div class='service-status down' id='status-" + sTeamId +  "-" + sServiceID + "'> "
            + '   <div class="service-att-def">'
            + '       <div class="service-att-def-row">'
            + '           <div class="service-att-def-cell defence-points" id="pt_def-' + sTeamId +  '-' + sServiceID + '">0.0</div>'
            + '           <div class="service-att-def-cell attack-points" id="pt_att-' + sTeamId +  '-' + sServiceID + '">0.0</div>'
            + '       </div>'
            + '       <div class="service-att-def-row">'
            + '           <div class="service-att-def-cell sla" id="sla-' + sTeamId +  '-' + sServiceID + '">0%</div>'
            + '           <div class="service-att-def-cell stollen-flags" id="att-' + sTeamId +  '-' + sServiceID + '">0</div>'
            + '       </div>'
            + '   </div>'
            + '  </div>'
            + "</div>\n";
        }
        sContent += ""
            + '   <div class="activity">'
            + '      <div class="activity-value" id="tries-' + sTeamId +  '"></div>'
            + '      <div class="activity-icon" id="tries-icon-' + sTeamId +  '"></div>'
            + '   </div>'
            + "</div>";
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
