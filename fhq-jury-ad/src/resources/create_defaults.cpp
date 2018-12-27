#include "create_defaults.h"
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <fs.h>


// ---------------------------------------------------------------------

bool CreateDefaults::make(const std::string &sWorkspace) {
    if (!CreateDefaults::createFolders(sWorkspace)) {
        return false;
    }
    return CreateDefaults::createFiles(sWorkspace);
}

// ---------------------------------------------------------------------

bool CreateDefaults::createFolders(const std::string &sWorkspace){
    
    // prepare folders
    std::vector<std::string> vCreateDirs;
    vCreateDirs.push_back(sWorkspace + "/logs");
    vCreateDirs.push_back(sWorkspace + "/teams");
    vCreateDirs.push_back(sWorkspace + "/checkers");
    vCreateDirs.push_back(sWorkspace + "/html");
    vCreateDirs.push_back(sWorkspace + "/html/css");
    vCreateDirs.push_back(sWorkspace + "/html/js");
    vCreateDirs.push_back(sWorkspace + "/html/images");
    vCreateDirs.push_back(sWorkspace + "/html/images/teams");
    vCreateDirs.push_back(sWorkspace + "/html/images/states");

    for(int i = 0; i < vCreateDirs.size(); i++) {
        std::string sPath = vCreateDirs[i];
        // check dir existing
        if (!FS::dirExists(sPath)) {
            // try make dir
            if (!FS::makeDir(sPath)) {
                return false;
            } else {
                std::cout << "Created folder " << sPath << std::endl;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------

bool CreateDefaults::createFiles(const std::string &sWorkspace) {

    // prepare default files
    struct LocalFileContent {
        LocalFileContent(const std::string &sPath, const std::string &sContent) {
            path = sPath;
            content = &sContent;
        };
        std::string path;
        const std::string *content;
    };

    std::vector<LocalFileContent> vCreateFiles;

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/game.conf", 
        CreateDefaults::g_sContent_game_conf
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/scoreboard.conf", 
        CreateDefaults::g_sContent_scoreboard_conf
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/mysql_storage.conf", 
        CreateDefaults::g_sContent_mysql_storage_conf
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/server.conf", 
        CreateDefaults::g_sContent_server_conf
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/html/index.html", 
        CreateDefaults::g_sContent_html_index_html
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/html/css/scoreboard.css", 
        CreateDefaults::g_sContent_html_css_scoreboard_css
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/html/js/scoreboard.js", 
        CreateDefaults::g_sContent_html_js_scoreboard_js
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/html/images/states/mumble.svg", 
        CreateDefaults::g_sContent_html_images_states_mumble_svg
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/html/images/states/down.svg", 
        CreateDefaults::g_sContent_html_images_states_down_svg
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/html/images/states/up.svg", 
        CreateDefaults::g_sContent_html_images_states_up_svg
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/html/images/states/shit.svg", 
        CreateDefaults::g_sContent_html_images_states_shit_svg
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/html/images/states/wait.svg", 
        CreateDefaults::g_sContent_html_images_states_wait_svg
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/html/images/states/corrupt.svg", 
        CreateDefaults::g_sContent_html_images_states_corrupt_svg
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/html/images/teams/unknown.svg", 
        CreateDefaults::g_sContent_html_images_teams_unknown_svg
    ));

    // write content of files
    for(int i = 0; i < vCreateFiles.size(); i++) {
        LocalFileContent lfc = vCreateFiles[i];
        // check file existing
        if (!FS::fileExists(lfc.path)) {
            if (!FS::writeFile(lfc.path, *lfc.content)) {
                return false;
            } else {
                std::cout << "Created file " << lfc.path << std::endl;
            }
        }
    }
	return true;
}

// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_game_conf
    = ""
    "# uniq gameid must be regexp [a-z0-9]+\n"
    "game.id = test\n"
    "\n"
    "# visible game name in scoreboard\n"
    "game.name = Test First Game\n"
    "\n"
    "# start time of game (UTC)\n"
    "game.start = 2018-09-06 12:00:00\n"
    "\n"
    "# end time of game (UTC)\n"
    "game.end = 2018-09-06 13:00:00\n"
    "\n"
    "# you can change flag time live (in minutes)\n"
    "game.flag_timelive_in_min = 10\n"
    "";

// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_scoreboard_conf
    = ""
    "# http port for scoreboard\n"
    "scoreboard.port = 8080\n"
    "\n"
    "# web page for scoreboard see index-template.html\n"
    "scoreboard.htmlfolder = ./html\n"
    "\n"
    "# If yes - will be random values in scoreboard\n"
    "scoreboard.random = no\n"
    ;

// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_mysql_storage_conf
    = ""
    "# if server.use_storage is mysql\n"
    "mysql_storage.dbhost = localhost\n"
    "mysql_storage.dbport = 3306\n"
    "mysql_storage.dbname = fhqjuryad\n"
    "mysql_storage.dbuser = fhqjuryad\n"
    "mysql_storage.dbpass = fhqjuryad\n"
    "";

// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_server_conf
    = ""
    "# use storage which storage will be used, now possible values:\n"
    "# mysql - will be used mysql database\n"
    "# ram - (not implemented yet) use just memory\n"
    "# postgres - (not implemented yet) will be used postgre database\n"
    "server.use_storage = mysql\n"
    ;

// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_html_index_html
    = ""
    "<html>\n"
    "<head>\n"
    "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />	\n"
    "    <title>Scoreboard</title>\n"
    "    <link rel=\"icon\" type=\"image/png\" sizes=\"32x32\" href=\"/favicon-32x32.png\">\n"
    "	<link rel=\"icon\" type=\"image/png\" sizes=\"16x16\" href=\"/favicon-16x16.png\">\n"
    "	<link rel=\"stylesheet\" href=\"css/scoreboard.css\">\n"
    "</head>\n"
    "<body>\n"
    "<center>\n"
    "	<div class=\"game-name\">{{game_name}}</div>\n"
    "	<div class=\"game-time-range\">{{game_time_range}}</div>\n"
    "	<div class=\"game-description\">Scoreboard</div>\n"
    "	<div id=\"scoreboard_content\" style=\"display: none\">\n"
    "		{{content}}\n"
    "	</div>\n"
    "	<div id=\"loader_content\" style=\"display: block\">\n"
    "		<div class=\"lds-facebook\"><div></div><div></div><div></div></div>\n"
    "	</div>\n"
    "</center>\n"
    "<script src=\"js/scoreboard.js\" defer=\"defer\"></script>\n"
    "</body>\n"
    "</html>\n";

// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_html_css_scoreboard_css
    = ""
    "body{\n"
    "    background-color: #000;\n"
    "    font: 12pt/10pt Arial;\n"
    "    color: #999;\n"
    "    padding: 0px;\n"
    "    margin: 0px;\n"
    "    background-color: #202020;\n"
    "    color: #aaa;\n"
    "}\n"
    "\n"
    "body,html{\n"
    "	  height: 100%;\n"
    "}\n"
    "\n"
    ".game-name{\n"
    "    font-size: 40px;\n"
    "    padding-top: 40px;\n"
    "    height: 40px;\n"
    "}\n"
    "\n"
    ".game-description{\n"
    "    padding-top: 10px;\n"
    "    padding-bottom: 20px;\n"
    "}\n"
    "\n"
    ".game-time-range {\n"
    "    color: #444;\n"
    "}\n"
    "\n"
    ".team-name{\n"
    "	font-size: 24px;\n"
    "}\n"
    "\n"
    ".team-ip{\n"
    "	font-size: 10px;\n"
    "}\n"
    "\n"
    "#scoreboard_content {\n"
    "  display: block;\n"
    "  position: relative;\n"
    "}\n"
    "\n"
    "#table_scoreboard {\n"
    "  width: 90%;\n"
    "  position: relative;\n"
    "}\n"
    "\n"
    ".hdrs {\n"
    "  border-bottom: 1px solid #111;\n"
    "  height: 50px;\n"
    "  position: absolute;\n"
    "  padding-top: 15px;\n"
    "  width: 100%;\n"
    "  white-space: nowrap;\n"
    "}\n"
    "\n"
    ".tm {\n"
    "  -webkit-transition: all 0.3s;\n"
    "  transition: all 0.3s;\n"
    "  height: 50px;\n"
    "  border-bottom: 1px solid #111;\n"
    "  padding-top: 15px;\n"
    "  position: absolute;\n"
    "  width: 100%;\n"
    "  white-space: nowrap;\n"
    "}\n"
    "\n"
    ".place {\n"
    "  display: inline-block;\n"
    "  font-size: 25px;\n"
    "  width: 50px;\n"
    "  text-align: center;\n"
    "  height: 50px;\n"
    "  vertical-align: top;\n"
    "  line-height: 50px;\n"
    "}\n"
    "\n"
    ".team-logo {\n"
    "  display: inline-block;\n"
    "  height: 50px;\n"
    "  width: 50px;\n"
    "  vertical-align: top;\n"
    "}\n"
    "\n"
    "img.team-logo {\n"
    "  width: 40px;\n"
    "	height: 40px;\n"
    "  padding-bottom: 10px;\n"
    "  padding-left: 5px;\n"
    "  padding-right: 5px;\n"
    "}\n"
    "\n"
    ".team {\n"
    "  display: inline-block;\n"
    "  height: 50px;\n"
    "  vertical-align: top;\n"
    "  width: 100px;\n"
    "  line-height: 25px;\n"
    "}\n"
    "\n"
    ".hdrs .team {\n"
    "  line-height: 50px;\n"
    "}\n"
    "\n"
    ".score {\n"
    "  display: inline-block;\n"
    "  height: 50px;\n"
    "  vertical-align: top;\n"
    "  text-align: center;\n"
    "  width: 100px;\n"
    "  line-height: 50px;\n"
    "}\n"
    "\n"
    ".service {\n"
    "  display: inline-block;\n"
    "  height: 50px;\n"
    "  vertical-align: top;\n"
    "  width: 150px;\n"
    "  line-height: 25px;\n"
    "  border-left: 1px solid #111;\n"
    "  padding-left: 15px;\n"
    "  padding-right: 15px;\n"
    "}\n"
    "\n"
    ".service-status {\n"
    "  display: inline-block;\n"
    "  height: 50px;\n"
    "  padding-left: 65px;\n"
    "  background-repeat: no-repeat;\n"
    "  background-size: 50px 50px;\n"
    "  transition: all 0.5s ease-out 0.5s;\n"
    "}\n"
    "\n"
    ".service-status.down {\n"
    "	background-image: url(\"../images/states/down.svg\");\n"
    "}\n"
    "\n"
    ".service-status.corrupt {\n"
    "	background-image: url(\"../images/states/corrupt.svg\");\n"
    "}\n"
    "\n"
    ".service-status.mumble {\n"
    "	background-image: url(\"../images/states/mumble.svg\");\n"
    "}\n"
    "\n"
    ".service-status.up {\n"
    "	background-image: url(\"../images/states/up.svg\");\n"
    "}\n"
    "\n"
    ".service-status.shit {\n"
    "	background-image: url(\"../images/states/shit.svg\");\n"
    "}\n"
    "\n"
    ".service-att-def, .service-sla {\n"
    "  height: 25px;\n"
    "  line-height: 25px;\n"
    "}\n"
    "\n"
    "/* loader */\n"
    ".lds-facebook {\n"
    "  display: inline-block;\n"
    "  position: relative;\n"
    "  width: 64px;\n"
    "  height: 64px;\n"
    "}\n"
    ".lds-facebook div {\n"
    "  display: inline-block;\n"
    "  position: absolute;\n"
    "  left: 6px;\n"
    "  width: 13px;\n"
    "  background: #fff;\n"
    "  animation: lds-facebook 1.2s cubic-bezier(0, 0.5, 0.5, 1) infinite;\n"
    "}\n"
    ".lds-facebook div:nth-child(1) {\n"
    "  left: 6px;\n"
    "  animation-delay: -0.24s;\n"
    "}\n"
    ".lds-facebook div:nth-child(2) {\n"
    "  left: 26px;\n"
    "  animation-delay: -0.12s;\n"
    "}\n"
    ".lds-facebook div:nth-child(3) {\n"
    "  left: 45px;\n"
    "  animation-delay: 0;\n"
    "}\n"
    "@keyframes lds-facebook {\n"
    "  0% {\n"
    "    top: 6px;\n"
    "    height: 51px;\n"
    "  }\n"
    "  50%, 100% {\n"
    "    top: 19px;\n"
    "    height: 26px;\n"
    "  }\n"
    "}\n";

// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_html_js_scoreboard_js
    = ""
    "\n"
    "// post request to server Async\n"
    "function getAjax (url, callback) {\n"
    "    callback = callback || function(){};\n"
    "	var tmpXMLhttp = null;\n"
    "	if (window.XMLHttpRequest) {\n"
    "		// code for IE7+, Firefox, Chrome, Opera, Safari\n"
    "		tmpXMLhttp = tmpXMLhttp || new window.XMLHttpRequest();\n"
    "	};\n"
    "	tmpXMLhttp.onreadystatechange=function() {\n"
    "		if (tmpXMLhttp.readyState==4) {\n"
    "			if (tmpXMLhttp.responseText == '') {\n"
    "                obj = { 'result' : 'fail' };\n"
    "                callback('fail', null);\n"
    "            } else {\n"
    "				try {\n"
    "					var obj = JSON.parse(tmpXMLhttp.responseText);\n"
    "                    callback(null, obj);\n"
    "                    delete obj;\n"
    "				} catch(e) {\n"
    "					console.error(e.name + ':' + e.message + '\\n stack:' + e.stack + '\\n' + tmpXMLhttp.responseText);\n"
    "				}\n"
    "				delete tmpXMLhttp;\n"
    "			}\n"
    "		}\n"
    "	}\n"
    "	tmpXMLhttp.open('GET', url, true);\n"
    "	tmpXMLhttp.send();\n"
    "};\n"
    "var scoreboard_content = document.getElementById('scoreboard_content');\n"
    "var loader_content = document.getElementById('loader_content');\n"
    "\n"
    "function updateUIValue(t, teamID, paramName){\n"
    "    var newValue = '';\n"
    "    if (paramName == 'score') {\n"
    "        newValue = t[paramName].toFixed(2);\n"
    "    } else {\n"
    "        newValue = '' + t[paramName];\n"
    "    }\n"
    "    var el = document.getElementById(teamID + '_' + paramName);\n"
    "    if (el) {\n"
    "        var prevVal = el.innerHTML;\n"
    "        if (prevVal != newValue) {\n"
    "            document.getElementById(teamID + '_' + paramName).innerHTML = newValue;\n"
    "        }\n"
    "    } else {\n"
    "        console.error('Not found element: ' + teamID + '_' + paramName);\n"
    "    }\n"
    "};\n"
    "\n"
    "var inSwitch = false;\n"
    "\n"
    "function switchUITeamRows(teamID1, teamID2){\n"
    "    console.log('switchUITeamRows: ' + teamID1 + ' <-> ' + teamID2);\n"
    "    if (inSwitch) {\n"
    "    /*    setTimeout(function(teamID1, teamID2){\n"
    "            switchUITeamRows(teamID1, teamID2);\n"
    "        },50);*/\n"
    "        return;\n"
    "    }\n"
    "    inSwitch = true;\n"
    "    var el1 = document.getElementById(teamID1);\n"
    "    var el2 = document.getElementById(teamID2);\n"
    "\n"
    "    el1.style.transform = 'translateY(100px)';\n"
    "    // el2.style.transform = 'translateY(-100px)';\n"
    "    setTimeout(function(){\n"
    "        el2.parentNode.insertBefore(el2, el1);\n"
    "        el1.style.transform = '';\n"
    "        el2.style.transform = '';\n"
    "        inSwitch = false;\n"
    "    },400);\n"
    "}\n"
    "\n"
    "setInterval(function(){\n"
    "    getAjax('/api/v1/scoreboard', function(err, resp){\n"
    "        if (err) {\n"
    "            scoreboard_content.style.display = 'none';\n"
    "            loader_content.style.display = 'block';\n"
    "            return;\n"
    "        }\n"
    "        var teamIDs = [];\n"
    "        for(var teamID in resp){\n"
    "            var t = resp[teamID];\n"
    "            teamIDs.push(teamID);\n"
    "            updateUIValue(t, teamID, 'place');\n"
    "            updateUIValue(t, teamID, 'score');\n"
    "\n"
    "            for(var sService in t){\n"
    "                if (sService == 'place' || sService == 'score') {\n"
    "                    continue;\n"
    "                }\n"
    "                var newState = t[sService]['status'];\n"
    "                var newAttack = t[sService]['attack'];\n"
    "                var newDefence = t[sService]['defence'];\n"
    "                var newSLA = t[sService]['sla'];\n"
    "                var el = document.getElementById(teamID + '_' + sService);\n"
    "                if (!el.classList.contains(newState)) {\n"
    "                    el.classList.remove('up');\n"
    "                    el.classList.remove('down');\n"
    "                    el.classList.remove('mumble');\n"
    "                    el.classList.remove('corrupt');\n"
    "                    el.classList.remove('shit');\n"
    "                    el.classList.add(newState);\n"
    "                }\n"
    "                document.getElementById(teamID + '_' + sService + '_ad').innerHTML = newDefence + ' / ' + newAttack;\n"
    "                document.getElementById(teamID + '_' + sService + '_sla').innerHTML = 'SLA: ' + newSLA + '%';\n"
    "            }\n"
    "        }\n"
    "\n"
    "        // sort by places\n"
    "        var elms2 = [];\n"
    "        var elms = document.getElementsByClassName('tm');\n"
    "        for(var i = 0; i < elms.length; i++){\n"
    "            var el1 = elms[i];\n"
    "            var place1 = parseInt(resp[el1.id]['place'], 10);\n"
    "            elms2.push({\n"
    "                e: elms[i],\n"
    "                p: place1\n"
    "            });\n"
    "        }\n"
    "        elms2.sort(function(a, b) {\n"
    "            return a.p - b.p;\n"
    "        });\n"
    "        for(var i = 0; i < elms2.length; i++){\n"
    "            elms2[i].e.style.top = ((i+1)*70) + 'px';\n"
    "        }\n"
    "        var bSorted = false;\n"
    "\n"
    "        while (!bSorted){\n"
    "            bSorted = true;\n"
    "            \n"
    "        }\n"
    "\n"
    "        // open controls\n"
    "        if (scoreboard_content.style.display != 'block') {\n"
    "            scoreboard_content.style.display = 'block'\n"
    "        }\n"
    "        if (loader_content.style.display != 'none') {\n"
    "            loader_content.style.display = 'none';\n"
    "        }\n"
    "    });\n"
    "}, 3000)\n";

// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_html_images_states_mumble_svg
    = ""
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
    "<!-- Created with Inkscape (http://www.inkscape.org/) by Toomai Glittershine for SmashWiki -->\n"
    "\n"
    "<svg\n"
    "   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
    "   xmlns:cc=\"http://creativecommons.org/ns#\"\n"
    "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
    "   xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"\n"
    "   xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n"
    "   id=\"svg2\"\n"
    "   version=\"1.1\"\n"
    "   height=\"256\"\n"
    "   width=\"256\"\n"
    "   viewBox=\"0 0 256 256\"\n"
    "   sodipodi:docname=\"mumble.svg\"\n"
    "   inkscape:version=\"0.92.3 (2405546, 2018-03-11)\">\n"
    "  <defs\n"
    "     id=\"defs8\" />\n"
    "  <sodipodi:namedview\n"
    "     pagecolor=\"#ffffff\"\n"
    "     bordercolor=\"#666666\"\n"
    "     borderopacity=\"1\"\n"
    "     objecttolerance=\"10\"\n"
    "     gridtolerance=\"10\"\n"
    "     guidetolerance=\"10\"\n"
    "     inkscape:pageopacity=\"0\"\n"
    "     inkscape:pageshadow=\"2\"\n"
    "     inkscape:window-width=\"1577\"\n"
    "     inkscape:window-height=\"796\"\n"
    "     id=\"namedview6\"\n"
    "     showgrid=\"false\"\n"
    "     inkscape:zoom=\"0.46093749\"\n"
    "     inkscape:cx=\"12.993576\"\n"
    "     inkscape:cy=\"306.45253\"\n"
    "     inkscape:window-x=\"145\"\n"
    "     inkscape:window-y=\"56\"\n"
    "     inkscape:window-maximized=\"0\"\n"
    "     inkscape:current-layer=\"svg2\" />\n"
    "  <metadata\n"
    "     id=\"metadata7\">\n"
    "    <rdf:RDF>\n"
    "      <cc:Work\n"
    "         rdf:about=\"\">\n"
    "        <dc:format>image/svg+xml</dc:format>\n"
    "        <dc:type\n"
    "           rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n"
    "        <dc:title></dc:title>\n"
    "      </cc:Work>\n"
    "    </rdf:RDF>\n"
    "  </metadata>\n"
    "  <path\n"
    "     style=\"opacity:1;fill:#d3868c;fill-opacity:1;stroke:none;stroke-width:2.76082492;stroke-linecap:round;stroke-linejoin:round;stroke-miterlimit:4;stroke-dasharray:none;stroke-dashoffset:0;stroke-opacity:1;paint-order:normal\"\n"
    "     d=\"m 126.33391,47.458251 c -60.616838,3e-6 -109.756507,35.936307 -109.756562,80.266009 3.9e-5,44.32971 49.139713,80.26603 109.756562,80.26603 60.61685,0 109.75653,-35.93632 109.75657,-80.26603 -6e-5,-44.329704 -49.13973,-80.266009 -109.75657,-80.266009 z m -1.94468,28.057426 c 12.27513,0.02511 24.09571,5.265978 33.12716,14.687535 -9.80153,-5.026083 -18.86638,-3.402274 -29.71864,-3.173783 7.46912,23.150741 -17.18156,24.007041 -0.0595,41.316411 l 7.66986,0.17746 0.22913,45.65969 c -3.72753,1.49326 -3.74671,6.45821 2.03418,6.95145 5.20848,-0.31248 5.0973,-5.37726 1.30166,-7.41699 V 128.4344 l 13.71946,-0.0886 v 32.14177 c -4.21496,1.50271 -3.3593,6.76783 1.58915,6.87375 5.52839,0.42706 5.6592,-5.87067 1.65176,-6.78486 V 128.7007 l 12.63955,-0.17745 0.32407,16.15961 c -1.68764,0.21373 -3.42359,0.85836 -3.24091,3.37421 0.37111,3.26858 1.92655,4.3868 3.90828,4.29643 -5.71398,24.31543 -22.97201,34.29616 -45.17523,34.32713 -27.086606,-1.1e-4 -49.044587,-24.8852 -49.044591,-55.58248 7e-6,-30.69728 21.957988,-55.582362 49.044591,-55.582473 z m 48.0728,52.918723 0.89164,0.0863 c 0.0444,0.85832 0.0713,1.71768 0.0805,2.57741 -0.0113,4.01333 -0.40613,8.01346 -1.17755,11.93043 z m -82.887919,0.76514 c -0.850146,11.56911 20.289049,12.1319 19.860499,0.25132 z\"\n"
    "     id=\"path828\"\n"
    "     inkscape:connector-curvature=\"0\"\n"
    "     sodipodi:nodetypes=\"scscscccccccccccccccccccsccccccccc\" />\n"
    "</svg>\n";

// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_html_images_states_down_svg
    = ""
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
    "<!-- Created with Inkscape (http://www.inkscape.org/) -->\n"
    "\n"
    "<svg\n"
    "   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
    "   xmlns:cc=\"http://creativecommons.org/ns#\"\n"
    "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
    "   xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"\n"
    "   xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n"
    "   width=\"300\"\n"
    "   height=\"300\"\n"
    "   viewBox=\"0 0 79.374998 79.375002\"\n"
    "   version=\"1.1\"\n"
    "   id=\"svg8\"\n"
    "   inkscape:version=\"0.92.2 (5c3e80d, 2017-08-06)\"\n"
    "   sodipodi:docname=\"down.svg\">\n"
    "  <defs\n"
    "     id=\"defs2\" />\n"
    "  <sodipodi:namedview\n"
    "     id=\"base\"\n"
    "     pagecolor=\"#000000\"\n"
    "     bordercolor=\"#666666\"\n"
    "     borderopacity=\"1.0\"\n"
    "     inkscape:pageopacity=\"0\"\n"
    "     inkscape:pageshadow=\"2\"\n"
    "     inkscape:zoom=\"0.7\"\n"
    "     inkscape:cx=\"208.34919\"\n"
    "     inkscape:cy=\"123.97809\"\n"
    "     inkscape:document-units=\"mm\"\n"
    "     inkscape:current-layer=\"layer1\"\n"
    "     showgrid=\"false\"\n"
    "     units=\"px\"\n"
    "     inkscape:window-width=\"1853\"\n"
    "     inkscape:window-height=\"1025\"\n"
    "     inkscape:window-x=\"67\"\n"
    "     inkscape:window-y=\"27\"\n"
    "     inkscape:window-maximized=\"1\" />\n"
    "  <metadata\n"
    "     id=\"metadata5\">\n"
    "    <rdf:RDF>\n"
    "      <cc:Work\n"
    "         rdf:about=\"\">\n"
    "        <dc:format>image/svg+xml</dc:format>\n"
    "        <dc:type\n"
    "           rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n"
    "        <dc:title></dc:title>\n"
    "      </cc:Work>\n"
    "    </rdf:RDF>\n"
    "  </metadata>\n"
    "  <g\n"
    "     inkscape:label=\"Layer 1\"\n"
    "     inkscape:groupmode=\"layer\"\n"
    "     id=\"layer1\"\n"
    "     transform=\"translate(0,-217.62498)\">\n"
    "    <path\n"
    "       style=\"fill:#de1500;fill-opacity:1;fill-rule:evenodd;stroke:none;stroke-width:0.99999994px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\"\n"
    "       d=\"M 151.35547 57.015625 C 149.55897 56.97751 147.79559 57.018819 146.07227 57.142578 C 69.005499 62.310629 12.148506 157.22025 93.572266 217.14258 L 93.339844 244.33594 L 114.11914 245.13086 L 112.85742 227.14258 L 122.5 228.21484 L 122.12109 244.8125 L 142.5 245.19141 L 142.14258 227.85742 L 151.07227 227.5 L 151.57617 244.93945 L 170.71484 244.64258 L 170.71484 227.5 L 179.64258 227.5 L 181.07227 244.64258 L 200.80078 244.87305 L 200.80078 216.66016 C 294.4666 135.97193 207.04689 58.197198 151.35547 57.015625 z M 153.29102 155.29492 L 224.50586 155.80078 C 222.35043 175.5956 213.14321 189.24799 190.16211 190.9043 C 171.33927 190.84496 156.06655 184.12512 153.29102 155.29492 z M 69.195312 155.80078 L 141.42188 156.30664 C 139.27936 177.48999 127.65414 191.85918 105.05664 191.91406 C 78.35974 190.05336 73.959604 171.75984 69.195312 155.80078 z M 146.97656 181.05469 L 166.42188 200.75195 L 128.03711 200.75195 L 146.97656 181.05469 z \"\n"
    "       transform=\"matrix(0.26458333,0,0,0.26458333,0,217.62498)\"\n"
    "       id=\"path826\" />\n"
    "  </g>\n"
    "</svg>\n"
    ;
    
// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_html_images_states_up_svg
    = ""
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
    "<!-- Generator: Adobe Illustrator 16.0.0, SVG Export Plug-In . SVG Version: 6.00 Build 0)  -->\n"
    "\n"
    "<svg\n"
    "   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
    "   xmlns:cc=\"http://creativecommons.org/ns#\"\n"
    "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
    "   xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"\n"
    "   xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n"
    "   version=\"1.1\"\n"
    "   id=\"Layer_1\"\n"
    "   x=\"0px\"\n"
    "   y=\"0px\"\n"
    "   width=\"512px\"\n"
    "   height=\"512px\"\n"
    "   viewBox=\"0 0 512 512\"\n"
    "   enable-background=\"new 0 0 512 512\"\n"
    "   xml:space=\"preserve\"\n"
    "   sodipodi:docname=\"up.svg\"\n"
    "   inkscape:version=\"0.92.3 (2405546, 2018-03-11)\"><metadata\n"
    "   id=\"metadata13\"><rdf:RDF><cc:Work\n"
    "       rdf:about=\"\"><dc:format>image/svg+xml</dc:format><dc:type\n"
    "         rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" /></cc:Work></rdf:RDF></metadata><defs\n"
    "   id=\"defs11\" /><sodipodi:namedview\n"
    "   pagecolor=\"#ffffff\"\n"
    "   bordercolor=\"#666666\"\n"
    "   borderopacity=\"1\"\n"
    "   objecttolerance=\"10\"\n"
    "   gridtolerance=\"10\"\n"
    "   guidetolerance=\"10\"\n"
    "   inkscape:pageopacity=\"0\"\n"
    "   inkscape:pageshadow=\"2\"\n"
    "   inkscape:window-width=\"1280\"\n"
    "   inkscape:window-height=\"996\"\n"
    "   id=\"namedview9\"\n"
    "   showgrid=\"false\"\n"
    "   inkscape:zoom=\"0.4609375\"\n"
    "   inkscape:cx=\"-143.56903\"\n"
    "   inkscape:cy=\"251.66102\"\n"
    "   inkscape:window-x=\"1920\"\n"
    "   inkscape:window-y=\"0\"\n"
    "   inkscape:window-maximized=\"1\"\n"
    "   inkscape:current-layer=\"Layer_1\" />\n"
    "<g\n"
    "   id=\"g6\"\n"
    "   style=\"fill:#00ce00;fill-opacity:1\"\n"
    "   transform=\"matrix(0.6830823,0,0,0.6830823,83.964673,83.281904)\">\n"
    "	<path\n"
    "   d=\"m 483.932,47.29 c -26.72,21.71 -55.73,42.46 -89.55,51.16 -20.05,5.17 -41.561,4.15 -61.05,-2.83 -32.49,-11.5 -59.341,-34.58 -91.771,-46.22 -19.03,-7.16 -39.91,-9.12 -59.95,-5.83 -29.68,4.86 -56.739,19.26 -81.62,35.58 -0.1,86.75 -0.039,173.51 -0.029,260.27 25.46,-16.641 53.189,-31.38 83.63,-35.891 21.93,-3.389 44.66,0.061 64.97,8.871 27.48,11.369 51.18,30.14 78.811,41.209 19.33,7.971 41,10.641 61.529,6.311 36.04,-7.58 66.9,-29.439 94.97,-52.29 0.22,-86.771 0.1,-173.56 0.06,-260.34 z\"\n"
    "   id=\"path2\"\n"
    "   style=\"fill:#00ce00;fill-opacity:1\"\n"
    "   inkscape:connector-curvature=\"0\" />\n"
    "	<path\n"
    "   d=\"M 73.5,426.784 V 73.216 c 9.064,-6.325 15,-16.824 15,-28.716 0,-19.33 -15.67,-35 -35,-35 -19.33,0 -35,15.67 -35,35 0,11.892 5.936,22.391 15,28.716 v 353.568 c -9.064,6.325 -15,16.824 -15,28.716 0,19.33 15.67,35 35,35 19.33,0 35,-15.67 35,-35 0,-11.892 -5.935,-22.391 -15,-28.716 z\"\n"
    "   id=\"path4\"\n"
    "   style=\"fill:#00ce00;fill-opacity:1\"\n"
    "   inkscape:connector-curvature=\"0\" />\n"
    "</g>\n"
    "</svg>"
    ;
    
// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_html_images_states_shit_svg
    = ""
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
    "<!-- Created with Inkscape (http://www.inkscape.org/) by Toomai Glittershine for SmashWiki -->\n"
    "\n"
    "<svg\n"
    "   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
    "   xmlns:cc=\"http://creativecommons.org/ns#\"\n"
    "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
    "   xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"\n"
    "   xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n"
    "   id=\"svg2\"\n"
    "   version=\"1.1\"\n"
    "   height=\"256\"\n"
    "   width=\"256\"\n"
    "   viewBox=\"0 0 256 256\"\n"
    "   sodipodi:docname=\"shit.svg\"\n"
    "   inkscape:version=\"0.92.3 (2405546, 2018-03-11)\">\n"
    "  <defs\n"
    "     id=\"defs8\" />\n"
    "  <sodipodi:namedview\n"
    "     pagecolor=\"#ffffff\"\n"
    "     bordercolor=\"#666666\"\n"
    "     borderopacity=\"1\"\n"
    "     objecttolerance=\"10\"\n"
    "     gridtolerance=\"10\"\n"
    "     guidetolerance=\"10\"\n"
    "     inkscape:pageopacity=\"0\"\n"
    "     inkscape:pageshadow=\"2\"\n"
    "     inkscape:window-width=\"1577\"\n"
    "     inkscape:window-height=\"796\"\n"
    "     id=\"namedview6\"\n"
    "     showgrid=\"false\"\n"
    "     inkscape:zoom=\"1.3037281\"\n"
    "     inkscape:cx=\"-34.551654\"\n"
    "     inkscape:cy=\"91.143768\"\n"
    "     inkscape:window-x=\"67\"\n"
    "     inkscape:window-y=\"27\"\n"
    "     inkscape:window-maximized=\"0\"\n"
    "     inkscape:current-layer=\"svg2\" />\n"
    "  <metadata\n"
    "     id=\"metadata7\">\n"
    "    <rdf:RDF>\n"
    "      <cc:Work\n"
    "         rdf:about=\"\">\n"
    "        <dc:format>image/svg+xml</dc:format>\n"
    "        <dc:type\n"
    "           rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n"
    "        <dc:title></dc:title>\n"
    "      </cc:Work>\n"
    "    </rdf:RDF>\n"
    "  </metadata>\n"
    "  <path\n"
    "     style=\"fill:#0400f5;fill-opacity:1;fill-rule:evenodd;stroke:none;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\"\n"
    "     d=\"m 61.737452,122.36758 c -19.180513,12.5062 -44.677646,29.24498 -17.5035,60.95445 44.510723,35.23308 137.842978,24.9914 165.571298,3.58793 39.65425,-23.93113 8.71562,-51.42129 -5.96611,-62.91526 -23.01118,54.87879 -146.988757,31.18468 -142.101688,-1.62712 z\"\n"
    "     id=\"path827\"\n"
    "     inkscape:connector-curvature=\"0\"\n"
    "     sodipodi:nodetypes=\"ccccc\" />\n"
    "  <path\n"
    "     style=\"fill:#0400f5;fill-opacity:1;fill-rule:evenodd;stroke:none;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\"\n"
    "     d=\"m 167.50016,87.113342 c 28.96793,16.391358 35.5038,32.359468 10.30509,47.728818 -27.35901,10.73965 -56.77173,9.8416 -87.190434,3.24295 C 70.907152,128.10797 59.572212,113.1026 77.241607,93.079446 98.888925,77.583052 127.5332,70.541554 136.04253,37.215037 c 24.77845,4.150054 36.45502,60.018899 -12.47457,65.084753 -3.80366,3.68253 -7.79432,8.70669 3.79661,10.84745 20.00577,3.25107 30.12183,-11.29934 40.13559,-26.033898 z\"\n"
    "     id=\"path829\"\n"
    "     inkscape:connector-curvature=\"0\"\n"
    "     sodipodi:nodetypes=\"cccccccc\" />\n"
    "</svg>\n"
    ;
    
// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_html_images_states_wait_svg
    = ""
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
    "<!-- Created with Inkscape (http://www.inkscape.org/) by Toomai Glittershine for SmashWiki -->\n"
    "\n"
    "<svg\n"
    "   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
    "   xmlns:cc=\"http://creativecommons.org/ns#\"\n"
    "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
    "   xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"\n"
    "   xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n"
    "   id=\"svg2\"\n"
    "   version=\"1.1\"\n"
    "   height=\"256\"\n"
    "   width=\"256\"\n"
    "   viewBox=\"0 0 256 256\"\n"
    "   sodipodi:docname=\"wait.svg\"\n"
    "   inkscape:version=\"0.92.3 (2405546, 2018-03-11)\">\n"
    "  <defs\n"
    "     id=\"defs8\" />\n"
    "  <sodipodi:namedview\n"
    "     pagecolor=\"#000000\"\n"
    "     bordercolor=\"#665666\"\n"
    "     borderopacity=\"1\"\n"
    "     objecttolerance=\"10\"\n"
    "     gridtolerance=\"10\"\n"
    "     guidetolerance=\"10\"\n"
    "     inkscape:pageopacity=\"0\"\n"
    "     inkscape:pageshadow=\"2\"\n"
    "     inkscape:window-width=\"1377\"\n"
    "     inkscape:window-height=\"779\"\n"
    "     id=\"namedview6\"\n"
    "     showgrid=\"false\"\n"
    "     inkscape:zoom=\"1.84375\"\n"
    "     inkscape:cx=\"90.456493\"\n"
    "     inkscape:cy=\"124.58161\"\n"
    "     inkscape:window-x=\"400\"\n"
    "     inkscape:window-y=\"93\"\n"
    "     inkscape:window-maximized=\"0\"\n"
    "     inkscape:current-layer=\"svg2\" />\n"
    "  <metadata\n"
    "     id=\"metadata7\">\n"
    "    <rdf:RDF>\n"
    "      <cc:Work\n"
    "         rdf:about=\"\">\n"
    "        <dc:format>image/svg+xml</dc:format>\n"
    "        <dc:type\n"
    "           rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n"
    "        <dc:title></dc:title>\n"
    "      </cc:Work>\n"
    "    </rdf:RDF>\n"
    "  </metadata>\n"
    "  <path\n"
    "     style=\"fill:#cbcaca;fill-opacity:1;fill-rule:evenodd;stroke:none;stroke-width:1.4105022px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\"\n"
    "     d=\"M 38.614923,54.661521 37.733183,177.92104 222.81539,177.50163 222.25465,54.994123 Z m 9.68645,13.289785 163.023087,-1.154194 1.79738,102.540328 -164.379599,-1.38253 z M 20.758217,180.34262 c 3.642632,6.71821 -0.983508,19.5343 13.89205,19.86175 H 226.08709 c 10.42092,-0.18046 9.82323,-12.32745 11.58215,-19.86175 h -81.91298 c -1.22001,2.21984 0.2496,7.57747 -5.2841,8.87498 h -42.2791 c -5.51863,-0.27788 -5.39804,-5.18901 -6.36259,-8.87498 z\"\n"
    "     id=\"path838\"\n"
    "     inkscape:connector-curvature=\"0\"\n"
    "     sodipodi:nodetypes=\"ccccccccccccccccccc\" />\n"
    "  <path\n"
    "     style=\"opacity:1;fill:#cbcaca;fill-opacity:1;stroke:none;stroke-width:20.12762642;stroke-linecap:round;stroke-linejoin:round;stroke-miterlimit:4;stroke-dasharray:none;stroke-dashoffset:0;stroke-opacity:1;paint-order:normal\"\n"
    "     d=\"m 130.82818,85.518284 a 37.47375,34.783398 0 0 0 -37.471731,34.783046 37.47375,34.783398 0 0 0 37.471731,34.78304 37.47375,34.783398 0 0 0 37.47443,-34.78304 37.47375,34.783398 0 0 0 -37.47443,-34.783046 z m -4.56117,10.500153 h 9.74308 l -0.74759,22.555213 14.98977,13.96324 -5.62183,5.72971 -17.23797,-16.11125 z\"\n"
    "     id=\"path866\"\n"
    "     inkscape:connector-curvature=\"0\" />\n"
    "</svg>\n"
    ;
    
// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_html_images_states_corrupt_svg
    = ""
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
    "<!-- Created with Inkscape (http://www.inkscape.org/) by Toomai Glittershine for SmashWiki -->\n"
    "\n"
    "<svg\n"
    "   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
    "   xmlns:cc=\"http://creativecommons.org/ns#\"\n"
    "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
    "   xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"\n"
    "   xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n"
    "   id=\"svg2\"\n"
    "   version=\"1.1\"\n"
    "   height=\"256\"\n"
    "   width=\"256\"\n"
    "   viewBox=\"0 0 256 256\"\n"
    "   sodipodi:docname=\"currupt.svg\"\n"
    "   inkscape:version=\"0.92.2 (5c3e80d, 2017-08-06)\">\n"
    "  <defs\n"
    "     id=\"defs8\" />\n"
    "  <sodipodi:namedview\n"
    "     pagecolor=\"#ffffff\"\n"
    "     bordercolor=\"#666666\"\n"
    "     borderopacity=\"1\"\n"
    "     objecttolerance=\"10\"\n"
    "     gridtolerance=\"10\"\n"
    "     guidetolerance=\"10\"\n"
    "     inkscape:pageopacity=\"0\"\n"
    "     inkscape:pageshadow=\"2\"\n"
    "     inkscape:window-width=\"1577\"\n"
    "     inkscape:window-height=\"796\"\n"
    "     id=\"namedview6\"\n"
    "     showgrid=\"false\"\n"
    "     inkscape:zoom=\"0.921875\"\n"
    "     inkscape:cx=\"128\"\n"
    "     inkscape:cy=\"128\"\n"
    "     inkscape:window-x=\"67\"\n"
    "     inkscape:window-y=\"27\"\n"
    "     inkscape:window-maximized=\"0\"\n"
    "     inkscape:current-layer=\"svg2\" />\n"
    "  <g\n"
    "     id=\"setting\"\n"
    "     style=\"fill:#f2de00;fill-opacity:1;stroke:#00e200;stroke-opacity:1\">\n"
    "    <animateTransform\n"
    "       attributeName=\"transform\"\n"
    "       attributeType=\"XML\"\n"
    "       type=\"rotate\"\n"
    "       from=\"0 128 128\"\n"
    "       to=\"360 128 128\"\n"
    "       dur=\"8s\"\n"
    "       repeatCount=\"indefinite\" />\n"
    "    <path\n"
    "       id=\"mainsymbolpath\"\n"
    "       style=\"opacity:1;fill:#f2de00;fill-opacity:1;stroke:#00e200;stroke-opacity:1\"\n"
    "       d=\"m 121.21875,37.84375 -3.4375,9.5 -1.75,9.59375 c -4.56422,0.763494 -8.98805,1.921834 -13.21875,3.5 l -6.28125,-7.375 -7.75,-6.53125 -11.71875,6.78125 1.78125,9.9375 3.21875,9.15625 c -3.522603,2.912618 -6.743632,6.133647 -9.65625,9.65625 L 63.25,78.84375 53.3125,77.0625 l -6.78125,11.71875 6.53125,7.75 7.375,6.28125 c -1.578166,4.2307 -2.736506,8.65453 -3.5,13.21875 l -9.59375,1.75 -9.5,3.4375 0,13.5625 9.5,3.4375 9.59375,1.75 c 0.763494,4.56422 1.921834,8.98805 3.5,13.21875 l -7.375,6.28125 -6.53125,7.75 6.78125,11.71875 9.9375,-1.78125 9.15625,-3.21875 c 2.912618,3.5226 6.133647,6.74363 9.65625,9.65625 l -3.21875,9.15625 -1.78125,9.9375 11.71875,6.78125 7.75,-6.53125 6.28125,-7.375 c 4.2307,1.57817 8.65453,2.73651 13.21875,3.5 l 1.75,9.59375 3.4375,9.5 13.5625,0 3.4375,-9.5 1.75,-9.59375 c 4.56422,-0.76349 8.98805,-1.92183 13.21875,-3.5 l 6.28125,7.375 7.75,6.53125 11.71875,-6.78125 -1.78125,-9.9375 -3.21875,-9.15625 c 3.5226,-2.91262 6.74363,-6.13365 9.65625,-9.65625 l 9.15625,3.21875 9.9375,1.78125 6.78125,-11.71875 -6.53125,-7.75 -7.375,-6.28125 c 1.57817,-4.2307 2.73651,-8.65453 3.5,-13.21875 l 9.59375,-1.75 9.5,-3.4375 0,-13.5625 -9.5,-3.4375 -9.59375,-1.75 c -0.76349,-4.56422 -1.92183,-8.98805 -3.5,-13.21875 l 7.375,-6.28125 6.53125,-7.75 -6.78125,-11.71875 -9.9375,1.78125 -9.15625,3.21875 c -2.91262,-3.522603 -6.13365,-6.743632 -9.65625,-9.65625 l 3.21875,-9.15625 1.78125,-9.9375 -11.71875,-6.78125 -7.75,6.53125 -6.28125,7.375 c -4.2307,-1.578166 -8.65453,-2.736506 -13.21875,-3.5 l -1.75,-9.59375 -3.4375,-9.5 -13.5625,0 z M 128,107.125 c 11.52306,0 20.875,9.35194 20.875,20.875 0,11.52306 -9.35194,20.875 -20.875,20.875 -11.52306,0 -20.875,-9.35194 -20.875,-20.875 0,-11.52306 9.35194,-20.875 20.875,-20.875 z\" />\n"
    "  </g>\n"
    "</svg>\n"
    ;
    
// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_html_images_teams_unknown_svg
    = ""
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
    "<!-- Created with Inkscape (http://www.inkscape.org/) -->\n"
    "\n"
    "<svg\n"
    "   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
    "   xmlns:cc=\"http://creativecommons.org/ns#\"\n"
    "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
    "   xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns=\"http://www.w3.org/2000/svg\"\n"
    "   xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"\n"
    "   xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n"
    "   width=\"300\"\n"
    "   height=\"300\"\n"
    "   viewBox=\"0 0 79.374998 79.375002\"\n"
    "   version=\"1.1\"\n"
    "   id=\"svg8\"\n"
    "   inkscape:version=\"0.92.3 (2405546, 2018-03-11)\"\n"
    "   sodipodi:docname=\"unknown.svg\">\n"
    "  <defs\n"
    "     id=\"defs2\" />\n"
    "  <sodipodi:namedview\n"
    "     id=\"base\"\n"
    "     pagecolor=\"#000000\"\n"
    "     bordercolor=\"#666666\"\n"
    "     borderopacity=\"1.0\"\n"
    "     inkscape:pageopacity=\"0\"\n"
    "     inkscape:pageshadow=\"2\"\n"
    "     inkscape:zoom=\"0.7\"\n"
    "     inkscape:cx=\"106.54607\"\n"
    "     inkscape:cy=\"251.26619\"\n"
    "     inkscape:document-units=\"mm\"\n"
    "     inkscape:current-layer=\"layer1\"\n"
    "     showgrid=\"false\"\n"
    "     units=\"px\"\n"
    "     inkscape:window-width=\"1154\"\n"
    "     inkscape:window-height=\"711\"\n"
    "     inkscape:window-x=\"648\"\n"
    "     inkscape:window-y=\"334\"\n"
    "     inkscape:window-maximized=\"0\"\n"
    "     inkscape:snap-global=\"false\" />\n"
    "  <metadata\n"
    "     id=\"metadata5\">\n"
    "    <rdf:RDF>\n"
    "      <cc:Work\n"
    "         rdf:about=\"\">\n"
    "        <dc:format>image/svg+xml</dc:format>\n"
    "        <dc:type\n"
    "           rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n"
    "        <dc:title></dc:title>\n"
    "      </cc:Work>\n"
    "    </rdf:RDF>\n"
    "  </metadata>\n"
    "  <g\n"
    "     inkscape:label=\"Layer 1\"\n"
    "     inkscape:groupmode=\"layer\"\n"
    "     id=\"layer1\"\n"
    "     transform=\"translate(0,-217.62498)\">\n"
    "    <ellipse\n"
    "       style=\"opacity:1;fill:#9997cb;fill-opacity:1;fill-rule:nonzero;stroke:none;stroke-width:0.48302105;stroke-linecap:round;stroke-linejoin:round;stroke-miterlimit:4;stroke-dasharray:none;stroke-dashoffset:0;stroke-opacity:1\"\n"
    "       id=\"path4577\"\n"
    "       cx=\"40.481434\"\n"
    "       cy=\"257.83157\"\n"
    "       rx=\"36.472393\"\n"
    "       ry=\"36.228432\" />\n"
    "    <path\n"
    "       style=\"fill:#000000;fill-opacity:1;fill-rule:evenodd;stroke:none;stroke-width:0.30725181px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\"\n"
    "       d=\"m 40.75428,222.44323 c -4.251864,0.11287 -8.346253,2.50272 -11.433128,6.44628 -6.522523,9.71081 -5.729056,17.14454 -0.76813,25.01882 l -3.401373,-2.41421 c -2.480143,2.63298 -6.295393,4.19438 -7.736505,7.7365 l -3.511191,10.97286 c -1.250713,4.37649 -0.440882,7.55851 2.852883,9.60161 l 7.022983,3.29216 0.309711,5.9909 33.632875,0.32337 0.348988,-6.47871 7.022983,-3.45658 c 3.620087,-2.57333 4.051148,-4.97214 3.346765,-7.40705 l -4.883024,-14.48464 c -1.382906,-2.47169 -3.738061,-4.56932 -7.022983,-6.30946 l -3.072518,2.4136 c 5.404071,-7.62053 4.817825,-16.4776 -0.329457,-24.03105 -3.717682,-5.16309 -8.127023,-7.32728 -12.378888,-7.21441 z m 0.472879,2.55043 c 4.962071,0.30143 12.836683,5.93826 13.771723,17.50254 -10.13189,-7.06172 -21.208832,-4.83083 -27.817092,0 0.169546,-8.18235 5.791796,-17.60811 14.045369,-17.50254 z m -5.9428,15.12014 c 1.178532,-0.0292 2.109593,0.62218 2.992703,1.40544 -1.623032,1.34855 -3.75953,2.17689 -4.69399,-0.73752 l -0.07801,-0.15483 c 0.656573,-0.34567 1.243605,-0.49983 1.7793,-0.51309 z m 11.088669,0.039 c 0.590999,0.009 1.189562,0.19608 1.797304,0.5905 -0.68186,2.8477 -3.129413,2.52373 -4.694594,0.69792 0.948327,-0.82778 1.912293,-1.30408 2.89729,-1.28842 z m 6.569308,8.58265 c -1.262737,5.03376 -4.577263,8.65632 -8.146972,12.10404 4.278011,-1.53848 8.527344,-2.28964 12.297873,-5.29981 1.650983,0.79753 2.263302,1.77788 3.709546,3.59914 l 4.196526,12.16086 c 0.293822,1.71354 1.12367,2.55869 -1.056331,4.0198 -1.438019,1.10832 -3.102578,2.21062 -4.948621,2.70691 l 0.697917,-11.76978 c -0.0816,-2.49183 -1.508794,-3.07722 -2.632644,-3.07373 l -32.768526,-0.0539 c -1.455783,0.0567 -2.021737,1.40124 -1.992335,2.49043 l 0.720721,12.4683 c -1.820762,-0.18255 -3.170325,-1.62792 -4.937146,-2.66586 -1.217077,-1.35718 -1.169509,-2.28327 -0.846482,-3.63955 l 4.019591,-12.03878 c 1.417921,-1.63595 2.350632,-2.83456 3.872005,-3.69462 4.815833,3.88642 8.938224,3.73153 11.714164,4.55785 -4.607094,-3.97998 -7.502336,-8.44276 -7.797716,-11.67736 9.957967,12.18501 14.031913,11.50564 23.89843,-0.19383 z\"\n"
    "       id=\"path4549\"\n"
    "       inkscape:connector-curvature=\"0\"\n"
    "       sodipodi:nodetypes=\"ccccccccccccccccccccccccccccccccccccccccccccccccccccc\" />\n"
    "    <g\n"
    "       transform=\"translate(1.8484103,17.681688)\"\n"
    "       id=\"layer1-3\"\n"
    "       inkscape:label=\"Layer 1\"\n"
    "       style=\"fill:#9997cb;fill-opacity:1;stroke:none;stroke-opacity:1\">\n"
    "      <path\n"
    "         id=\"path826\"\n"
    "         d=\"m 39.535499,251.38304 c -0.122391,-0.003 -0.242528,2.2e-4 -0.359934,0.009 -5.250411,0.35209 -9.123968,6.81809 -3.576724,10.90049 l -0.01584,1.85263 1.415654,0.0541 -0.08596,-1.2255 0.65693,0.0731 -0.02582,1.13076 1.388377,0.0258 -0.02435,-1.18093 0.608362,-0.0243 0.03434,1.18813 1.303879,-0.0203 v -1.16789 h 0.608231 l 0.0974,1.16789 1.344066,0.0157 v -1.92208 c 6.381271,-5.49714 0.425535,-10.79579 -3.368614,-10.87629 z m 0.131866,6.69558 4.851729,0.0345 c -0.146845,1.34858 -0.774116,2.27869 -2.339773,2.39153 -1.282363,-0.004 -2.322865,-0.46185 -2.511956,-2.42599 z m -5.729279,0.0345 4.920657,0.0345 c -0.145965,1.44318 -0.93797,2.42213 -2.477493,2.42586 -1.818809,-0.12676 -2.118581,-1.37306 -2.443164,-2.46032 z m 5.299087,1.7205 1.324772,1.34193 h -2.61508 z\"\n"
    "         style=\"fill:#9997cb;fill-opacity:1;fill-rule:evenodd;stroke:none;stroke-width:0.06812806px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\"\n"
    "         inkscape:connector-curvature=\"0\" />\n"
    "    </g>\n"
    "  </g>\n"
    "</svg>";

// ---------------------------------------------------------------------
