var NeedsTimingInfo = true;
const PPQN = 24;
var ticksPerPulse;
var event;
var isPlaying = false;
const msPerMinute = 60000;
const tickNote = 4;
const stopNote = 3;
const channel = 15;
const velocity = 1;

function ProcessMIDI() {
    var info = GetTimingInfo();

    if (info.playing) {

        if (!isPlaying) {
            // Starting
            isPlaying = true;
            ticksPerPulse = msPerMinute / Math.round(info.tempo);

            var start = new NoteOn;
            start.ticks = 0;
            start.beatPos = ticks2Beats(0);
            start.channel = MIDI.normalizeChannel(channel);
            start.pitch = MIDI.normalizeData(tickNote);
            start.velocity = MIDI.normalizeData(velocity);
            event = start;
        }

        var ev = event;

        // Check if the next event position is within the curently processed block (it most likely is) 
        if (ev.beatPos >= info.blockStartBeat && ev.beatPos <= info.blockEndBeat) {

            // Schedule event
            ev.sendAtBeat(ev.beatPos);

            // Create next pulse
            var ticks = ev.ticks + ticksPerPulse / PPQN;
            var pulse = new NoteOn;
            pulse.ticks = ticks;
            pulse.beatPos = ticks2Beats(ticks);
            pulse.channel = MIDI.normalizeChannel(channel);
            pulse.pitch = MIDI.normalizeData(tickNote);
            pulse.velocity = MIDI.normalizeData(velocity);
            event = pulse;
        }
    }
    else {
        if (isPlaying) {
            // Stopping
            isPlaying = false;

            let stop = new NoteOn;
            stop.channel = MIDI.normalizeChannel(channel);
            stop.pitch = MIDI.normalizeData(stopNote);
            stop.velocity = MIDI.normalizeData(velocity);
            stop.send();
        }
    }
}

function ticks2Beats(ticks) {
    return (parseInt(ticks) / ticksPerPulse) + 1;
}