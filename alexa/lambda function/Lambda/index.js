/**
    Copyright 2014-2015 Amazon.com, Inc. or its a`f`filiates. All Rights Reserved.

    Licensed under the Apache License, Version 2.0 (the "License"). You may not use this file except in compliance with the License. A copy of the License is located at

        http://aws.amazon.com/apache2.0/

    or in the "license" file accompanying this file. This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
*/

/**
 * App ID for the skill
 */
var APP_ID = undefined; //replace with "amzn1.echo-sdk-ams.app.[your-unique-value-here]";

var config = {};
config.IOT_BROKER_ENDPOINT      = "Enter your information"
config.IOT_BROKER_REGION        = "us-east-1";
config.IOT_THING_NAME           = "Pi";

var AWS = require('aws-sdk');
AWS.config.region = config.IOT_BROKER_REGION;

var iotData = new AWS.IotData({endpoint: config.IOT_BROKER_ENDPOINT});

var AlexaSkill = require('./AlexaSkill');

var HelloWorld = function () {
    AlexaSkill.call(this, APP_ID);
};

HelloWorld.prototype = Object.create(AlexaSkill.prototype);
HelloWorld.prototype.constructor = HelloWorld;

HelloWorld.prototype.eventHandlers.onSessionStarted = function (sessionStartedRequest, session) {
    console.log("HelloWorld onSessionStarted requestId: " + sessionStartedRequest.requestId
        + ", sessionId: " + session.sessionId);
};

HelloWorld.prototype.eventHandlers.onLaunch = function (launchRequest, session, response) {
    console.log("HelloWorld onLaunch requestId: " + launchRequest.requestId + ", sessionId: " + session.sessionId);
    var speechOutput = "Welcome to the smart picture frame, you can say cheese";
    var repromptText = "You can say cheese";
    response.ask(speechOutput, repromptText);
};

HelloWorld.prototype.eventHandlers.onSessionEnded = function (sessionEndedRequest, session) {
    console.log("HelloWorld onSessionEnded requestId: " + sessionEndedRequest.requestId
        + ", sessionId: " + session.sessionId);
};

HelloWorld.prototype.intentHandlers = {
    "TakePictureIntent": function (intent, session, response) {
        var repromptText = "";
        var sessionAttributes = {};
        var shouldEndSession = false;
        var speechOutput = "";
	var command = {"take": 1, "prev": 0, "next": 0};
        var paramsUpdate = {
            topic:"/Pi",
            payload: JSON.stringify(command),
            qos:0
        };
        iotData.publish(paramsUpdate, function(err, data) {
          if (err){
            console.log("MQTT Error" + data);
          }
          else {
            speechOutput = "Taking picture";
            console.log(data);
            response.ask(speechOutput, repromptText);
	        }    
        });
    },
    "NextPictureIntent": function (intent, session, response) {
        var repromptText = "";
        var sessionAttributes = {};
        var shouldEndSession = false;
        var speechOutput = "";
	var command = {"take": 0, "prev": 0, "next": 1};
        var paramsUpdate = {
            topic:"/Pi",
            payload: JSON.stringify(command),
            qos:0
        };
        iotData.publish(paramsUpdate, function(err, data) {
          if (err){
            console.log("MQTT Error" + data);
          }
          else {
            speechOutput = "Displaying next picture";
            console.log(data);
            response.ask(speechOutput, repromptText);
          }    
        });
    },
    "PrevPictureIntent": function (intent, session, response) {
        var repromptText = "";
        var sessionAttributes = {};
        var shouldEndSession = false;
        var speechOutput = "";
	var command = {"take": 0, "prev": 1, "next": 0};
        var paramsUpdate = {
            topic:"/Pi",
            payload: JSON.stringify(command),
            qos:0
        };
        iotData.publish(paramsUpdate, function(err, data) {
          if (err){
            console.log("MQTT Error" + data);
          }
          else {
            speechOutput = "Displaying previous picture";
            console.log(data);
            response.ask(speechOutput, repromptText);
          }    
        });
    },
    "AMAZON.HelpIntent": function (intent, session, response) {
        response.ask("You can ask me to take picture", "You can ask me to take picture");
    }
};

exports.handler = function (event, context) {
    var helloWorld = new HelloWorld();
    helloWorld.execute(event, context);
};
