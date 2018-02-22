// Beginning of Button Clicks and Helper Functions

// Login to Control Button
document.getElementById('login').addEventListener('click', controlPage);
document.getElementById('login').addEventListener('click', loginStream);
document.getElementById('login').addEventListener('click', writeStatus);

// Brew-Off Buttons
document.getElementById('brewbutton').addEventListener('click', toBrew);
document.getElementById('donebutton').addEventListener('click', toDone);

// Coffee Strength Buttons
document.getElementById('coffeedecaf').addEventListener('click', decafCoffee);
document.getElementById('coffeemediuem').addEventListener('click', mediumCoffee);
document.getElementById('coffeedark').addEventListener('click', darkCoffee);

// Enable-Disable Timer, Start Timer Buttons
document.getElementById('enablebutton').addEventListener('click', enableButton);
document.getElementById('startbutton').addEventListener('click', coffeeTimer);

// Control to Login Button
document.getElementById('controltologin').addEventListener('click', loginPage);

// Loads Login Page
function load() {
    loginPage();
}

// Sets Login to Visible, Control to Hidden
function loginPage() {
    document.getElementById('loginpage').hidden = false;
    document.getElementById('controlpage').hidden = true;
}

// Sets Control to Visible, Login to Hidden
function controlPage() {
    document.getElementById('loginpage').hidden = true;
    document.getElementById('controlpage').hidden = false;
}

// Switches Enable-Disable Timer
function enableButton() {
    if(document.getElementById('enablebutton').value === "Enable") {
        document.getElementById('enablebutton').value = "Disable";
    } else {
        document.getElementById('enablebutton').value = "Enable";
    }
}
// End of Button Clicks and Helper Functions


// Beginning of Particle Object Instantiation and Particle Login
// Creates new Particle Object
var particle = new Particle();
var token;

var deviceId = "190027001147343438323536"

var login = 'scotthom@wustl.edu'
var password = 'everettscott'

// generates token on a successful login so we can call particle functions using that token
function loginSuccess(data) {
    console.log('API call completed on promise resolve: ', data.body.access_token);
    token = data.body.access_token;
}

function loginError(error) {
    console.log('API call completed on promise fail: ', error);
}

// logs into particle using username and password associated with our device
particle.login({username: login, password: password}).then(loginSuccess, loginError);

function callSuccess(data) {
    console.log('Function called succesfully:', data);
}

function callFailure(error) {
    console.log('An error occurred:', error);
}
// End of Particle Object Instantiation and Particle Login


// Beginning of Particle Functions
// boolean updateChoice checks whether or not user can update coffee preference (depending if machine is on or off)
var updateChoice;

// Calls Particle Function writeStatus (fetches current states from photon)
function writeStatus() {
    particle.callFunction({ deviceId: deviceId, name: 'writeStatus', argument: 'writeStatus', auth: token}).then(callSuccess, callFailure);
}

// Calls Particle Function toOn (turns machine on)
function toBrew() {
    particle.callFunction({ deviceId: deviceId, name: 'toOn', argument: 'toOn', auth: token }).then(callSuccess, callFailure);
}

// Calls Particle Function toOff (turns machine off)
function toDone() {
    particle.callFunction({ deviceId: deviceId, name: 'toOff', argument: 'toOff', auth: token }).then(callSuccess, callFailure);
}

// Calls Particle Function toDecaf (selects preference decaf)
function decafCoffee() {
    if (updateChoice === true) {
        particle.callFunction({ deviceId: deviceId, name: 'toDecaf', argument: 'toDecaf', auth: token }).then(callSuccess, callFailure);
    }
}

// Calls Particle Function toMedium (selects preference medium)
function mediumCoffee() {
    if (updateChoice === true) {
        particle.callFunction({ deviceId: deviceId, name: 'toMedium', argument: 'toMedium', auth: token }).then(callSuccess, callFailure);
    }
}

// Calls Particle Function toDark (selects preference dark)
function darkCoffee() {
    if (updateChoice === true) {
        particle.callFunction({ deviceId: deviceId, name: 'toDark', argument: 'toDark', auth: token }).then(callSuccess, callFailure);
    }
}

// Calls Particle Function toTimer (particle handles timing)
function coffeeTimer() {
    let timeCoffee = document.getElementById('coffeetime').value + "";
    let actualTime = timeCoffee*1000;

    if (document.getElementById('enablebutton').value === "Disable") {
        if (timeCoffee < 3 || timeCoffee > 101) {
            alert("The time you have entered is not in the legal range");
        } else {
            particle.callFunction({ deviceId: deviceId, name: 'toTimer', argument: timeCoffee, auth: token }).then(callSuccess, callFailure);
        }
    } else {
        alert("You have disabled this feature");
    }
}
// End of Particle Functions


// Beginning of Particle Event Stream and Update Functions
// coffee reads the value of the state coming in from the photon
var coffee;

// Begins reading data from the photon in order to keep status of both the UI and Photon in the same states
function loginStream() {
    particle.getEventStream({ deviceId: deviceId, auth: token}).then(function(stream) {
        stream.on('event', function(data) {
            coffee = data.data;

            updateCoffeePreference();
            updateCoffeeStatus();
            updateCoffeeTouch();
        });
    });
}

// changes coffee preference based on status coming in from the photon
function updateCoffeePreference() {

    if(coffee === "decaf") {
        document.getElementById('coffeetype').innerHTML = "Coffee Strength: Decaf";
    } else if(coffee === "medium") {
        document.getElementById('coffeetype').innerHTML = "Coffee Strength: Medium";
    } else if(coffee === "dark") {
        document.getElementById('coffeetype').innerHTML = "Coffee Strength: Dark";
    }

}

// changes coffee status based on status coming in from the photon
function updateCoffeeStatus() {

    if(coffee === "on") {
        document.getElementById('stat').innerHTML = "Status: Brewing";
        updateChoice = false;
    } else if (coffee === "off") {
        document.getElementById('stat').innerHTML = "Status: Off";
        updateChoice = true;
    }

}

// changes coffee touch based on status coming in from the photon
function updateCoffeeTouch() {
    if(coffee === "safe") {
        document.getElementById('currenttoucuh').innerHTML = "Safe";
    } else if(coffee === "unsafe") {
        document.getElementById('currenttoucuh').innerHTML = "Unsafe";
    }
}
// End of Particle Event Stream and Update Functions
