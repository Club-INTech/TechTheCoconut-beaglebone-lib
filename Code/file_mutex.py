# -*- coding:utf-8 -*-

# Convention : Mutex à 1 <-> PAS POSSIBLE D'ECRIRE.

# Classe de mutex de file
class MutexFile :
    def __init__(self):
        # Dictionnaire type "nomFichier" = MutexOk/PasOk
        if not hasattr(MutexFile, "mutex") :
            MutexFile.mutex = {}
            
    def __str__(self) :
        return str(MutexFile.mutex)
        
    def addMutex(self, nom) :
        # Ajout du mutex s'il n'existe pas déjà.
        if not nom in MutexFile.mutex :
            MutexFile.mutex.update({nom:False})
        
    def getMutex(self, nom) :
        return MutexFile.mutex[nom]
        
    def setMutex(self, nom, etat) :
        MutexFile.mutex[nom] = etat
    
class File :
    def __init__(self, nom) :
        if not hasattr(File, "mutex") :
            File.mutex = MutexFile()
        
        self.nom = nom
        
        # Ouverture du fichier
        try :
            self.fichier = open(self.nom, "w")
            File.mutex.addMutex(self.nom)
        except :
            print "Impossible d'ouvrir le fichier " + nom + "."
            
        
    def write(self, message) :
        # On checke le mutex
        if not File.mutex.getMutex(self.nom) :
            print "Ecriture du message..."
            File.mutex.setMutex(self.nom, True)
            self.fichier.write(message)
            self.fichier.flush()
            File.mutex.setMutex(self.nom, False)
        