#ifndef ACRONYMS_H
#define ACRONYMS_H

#include <unordered_map>
#include <string>

// Acronym dictionary for Neuroscience and Cognitive Behavioral Therapy
static const std::unordered_map<std::string, std::string> acronyms = {
    // Neuroscience
    {"fMRI", "Functional Magnetic Resonance Imaging"},
    {"EEG", "Electroencephalography"},
    {"MEG", "Magnetoencephalography"},
    {"PET", "Positron Emission Tomography"},
    {"TMS", "Transcranial Magnetic Stimulation"},
    {"CNS", "Central Nervous System"},
    {"PNS", "Peripheral Nervous System"},
    {"ANS", "Autonomic Nervous System"},
    {"GABA", "Gamma-Aminobutyric Acid"},
    {"NMDA", "N-Methyl-D-Aspartate"},
    // Cognitive Behavioral Therapy
    {"CBT", "Cognitive Behavioral Therapy"},
    {"DBT", "Dialectical Behavior Therapy"},
    {"ACT", "Acceptance and Commitment Therapy"},
    {"REBT", "Rational Emotive Behavior Therapy"},
    {"ERP", "Exposure and Response Prevention"},
    {"Socratic Questioning", "A technique used in CBT to challenge automatic thoughts"},
    {"Cognitive Restructuring", "A process of identifying and changing negative thought patterns"},
};

#endif // ACRONYMS_H
