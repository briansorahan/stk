#include "stk.hpp"
#include "dsp/samplerate.hpp"
#include "dsp/ringbuffer.hpp"
#include "dsp/filter.hpp"


#define HISTORY_SIZE (1<<21)

struct ModalBar : Module {
	enum ParamIds {
		HARDNESS_PARAM,
		POSITION_PARAM,
		PRESET_PARAM,
		MODULATION_PARAM,
		CC_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		HARDNESS_INPUT,
		POSITION_INPUT,
		PRESET_INPUT,
		MODULATION_INPUT,
		CC_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_OUTPUT,
		NUM_OUTPUTS
	};

	ModalBar() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}

	void step() override;
};


void ModalBar::step() {
	// // Get input to delay block
	// float in = inputs[IN_INPUT].value;
	// float feedback = clampf(params[FEEDBACK_PARAM].value + inputs[FEEDBACK_INPUT].value / 10.0, 0.0, 1.0);
	// float dry = in + lastWet * feedback;

	// // Compute delay time in seconds
	// float delay = 1e-3 * powf(10.0 / 1e-3, clampf(params[TIME_PARAM].value + inputs[TIME_INPUT].value / 10.0, 0.0, 1.0));
	// // Number of delay samples
	// float index = delay * engineGetSampleRate();

	// // TODO This is a horrible digital delay algorithm. Rewrite later.

	// // Push dry sample into history buffer
	// if (!historyBuffer.full()) {
	// 	historyBuffer.push(dry);
	// }

	// // How many samples do we need consume to catch up?
	// float consume = index - historyBuffer.size();
	// // printf("%f\t%d\t%f\n", index, historyBuffer.size(), consume);

	// // printf("wanted: %f\tactual: %d\tdiff: %d\tratio: %f\n", index, historyBuffer.size(), consume, index / historyBuffer.size());
	// if (outBuffer.empty()) {
	// 	// Idk wtf I'm doing
	// 	double ratio = 1.0;
	// 	if (consume <= -16)
	// 		ratio = 0.5;
	// 	else if (consume >= 16)
	// 		ratio = 2.0;

	// 	// printf("%f\t%lf\n", consume, ratio);
	// 	int inFrames = mini(historyBuffer.size(), 16);
	// 	int outFrames = outBuffer.capacity();
	// 	// printf(">\t%d\t%d\n", inFrames, outFrames);
	// 	src.setRatioSmooth(ratio);
	// 	src.process((const Frame<1>*)historyBuffer.startData(), &inFrames, (Frame<1>*)outBuffer.endData(), &outFrames);
	// 	historyBuffer.startIncr(inFrames);
	// 	outBuffer.endIncr(outFrames);
	// 	// printf("<\t%d\t%d\n", inFrames, outFrames);
	// 	// printf("====================================\n");
	// }

	// float wet = 0.0;
	// if (!outBuffer.empty()) {
	// 	wet = outBuffer.shift();
	// }

	// // Apply color to delay wet output
	// // TODO Make it sound better
	// float color = clampf(params[COLOR_PARAM].value + inputs[COLOR_INPUT].value / 10.0, 0.0, 1.0);
	// float lowpassFreq = 10000.0 * powf(10.0, clampf(2.0*color, 0.0, 1.0));
	// lowpassFilter.setCutoff(lowpassFreq / engineGetSampleRate());
	// lowpassFilter.process(wet);
	// wet = lowpassFilter.lowpass();
	// float highpassFreq = 10.0 * powf(100.0, clampf(2.0*color - 1.0, 0.0, 1.0));
	// highpassFilter.setCutoff(highpassFreq / engineGetSampleRate());
	// highpassFilter.process(wet);
	// wet = highpassFilter.highpass();

	// lastWet = wet;

	// float mix = clampf(params[MIX_PARAM].value + inputs[MIX_INPUT].value / 10.0, 0.0, 1.0);
	// float out = crossf(in, wet, mix);
	// outputs[OUT_OUTPUT].value = out;
}


ModalBarWidget::ModalBarWidget() {
	ModalBar *module = new ModalBar();
	setModule(module);
	box.size = Vec(15*8, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/ModalBar.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x-30, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x-30, 365)));

	addParam(createParam<RoundBlackKnob>(Vec(67, 57), module, ModalBar::HARDNESS_PARAM, 0.0, 1.0, 0.5));
	addParam(createParam<RoundBlackKnob>(Vec(67, 123), module, ModalBar::POSITION_PARAM, 0.0, 1.0, 0.5));
	addParam(createParam<RoundBlackKnob>(Vec(67, 190), module, ModalBar::PRESET_PARAM, 0.0, 1.0, 0.5));
	addParam(createParam<RoundBlackKnob>(Vec(67, 190), module, ModalBar::MODULATION_PARAM, 0.0, 1.0, 0.5));
	addParam(createParam<RoundBlackKnob>(Vec(67, 257), module, ModalBar::CC_PARAM, 0.0, 1.0, 0.5));

	addInput(createInput<PJ301MPort>(Vec(14, 63), module, ModalBar::HARDNESS_INPUT));
	addInput(createInput<PJ301MPort>(Vec(14, 129), module, ModalBar::POSITION_INPUT));
	addInput(createInput<PJ301MPort>(Vec(14, 196), module, ModalBar::PRESET_INPUT));
	addInput(createInput<PJ301MPort>(Vec(14, 263), module, ModalBar::MODULATION_INPUT));
	addInput(createInput<PJ301MPort>(Vec(14, 320), module, ModalBar::CC_INPUT));
	addOutput(createOutput<PJ301MPort>(Vec(73, 320), module, ModalBar::OUT_OUTPUT));
}
