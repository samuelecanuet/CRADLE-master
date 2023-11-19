#include <iostream>
#include <fstream>
#include <sstream>
#include "TFile.h"
#include "TTree.h"

#define PROTON 2212
#define NEUTRINO_ELECTRON 12
#define ANTI_NEUTRINO_ELECTRON -12
#define POSITRON  -11
#define ELECTRON 11
#define CL32 1000180320
#define AR32 1000180400
#define S32 1000160320
#define S31 1000160310
#define CL31 1000180310

struct ParticleData {
    int event;
    int particle;
    double time;
    int code;
    double excitation_energy;
    double kinetic_energy;
    double p;
    double px;
    double py;
    double pz;
};

int main() {
    std::map<std::string, int> particleToPDGCode;

    // Ajout des associations nom de particule - code PDG à la map
    particleToPDGCode["p"] = 2212;
    particleToPDGCode["enu"] = 12;
    particleToPDGCode["enubar"] = -12;
    particleToPDGCode["e+"] = -11;
    particleToPDGCode["e-"] = 11;
    particleToPDGCode["32Cl"] = 1000180320;
    particleToPDGCode["32Ar"] = 1000180400;
    particleToPDGCode["32S"] = 1000160320;
    particleToPDGCode["32S"] = 1000160310;
    particleToPDGCode["31Cl"] = 1000180310;
    particleToPDGCode["gamma"] = 22;
    // Ouverture du fichier texte
    std::ifstream file("test.txt");
    if (!file.is_open()) {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
        return 1;
    }

    // Création du fichier root et du TTree
    TFile outputFile("output.root", "RECREATE");
    TTree tree("ParticlesTree", "Tree for Particle Data");

    // Déclaration des variables pour stocker les données
    ParticleData particleData;

    // Ajout des branches au TTree
    tree.Branch("event", &particleData.event);
    tree.Branch("particle", &particleData.particle);
    tree.Branch("time", &particleData.time);
    tree.Branch("code", &particleData.code);
    tree.Branch("excitation_energy", &particleData.excitation_energy);
    tree.Branch("kinetic_energy", &particleData.kinetic_energy);
    tree.Branch("p", &particleData.p);
    tree.Branch("px", &particleData.px);
    tree.Branch("py", &particleData.py);
    tree.Branch("pz", &particleData.pz);

    std::string line, name;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        if (line.empty()) {
            continue; // Ignore les lignes vides
        }

        int eventType, particleType;
        if (!(iss >> eventType >> particleType)) {
            continue; // Ignore les lignes incorrectes
        }

        // Si c'est une ligne de particule, stockez les informations dans le TTree
        if (particleType == 0) {
            iss >> particleData.time >> name >> particleData.excitation_energy >>
                particleData.kinetic_energy >> particleData.p >>particleData.px >> particleData.py >> particleData.pz;
            particleData.code = particleToPDGCode[name];
            std::cout<<name<<std::endl;
            particleData.event = eventType;
            particleData.particle = particleType;

            // Remplissage du TTree avec les données de la particule
            tree.Fill();
        }
    }

    // Écriture du TTree dans le fichier root
    outputFile.Write();
    outputFile.Close();

    // Fermeture du fichier texte
    file.close();

    return 0;
}
