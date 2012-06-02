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
        
    # Retourne True si le mutex pour ce fichier existait déjà, False sinon
    def addMutex(self, nom) :
        # Ajout du mutex s'il n'existe pas déjà.
        if not nom in MutexFile.mutex :
            MutexFile.mutex.update({nom:False})
            return False
        return True
            
        
    def getMutex(self, nom) :
        return MutexFile.mutex[nom]
        
    def setMutex(self, nom, etat) :
        MutexFile.mutex[nom] = etat
    
class File :
    # L'argument deleteAll détermine si oui ou non on veut supprimer les données
    # contenues dans le fichier avant utilisation. (ne peut se faire que pour la
    # première instance de ce fichier).
    def __init__(self, nom, deleteAll = True) :
        if not hasattr(File, "mutex") :
            File.mutex = MutexFile()
            File.pattern = "\n"
        
        self.nom = nom
        
        # Ouverture du fichier
        try :
            dejaOuvert = File.mutex.addMutex(self.nom)
            if (not dejaOuvert) and deleteAll: 
                self.fichier = open(self.nom, "w")
            else :
                self.fichier = open(self.nom, "a")
            
        except :
            print "Impossible d'ouvrir le fichier " + nom + "."
            
        
    def write(self, message) :
        # On checke le mutex
        if not File.mutex.getMutex(self.nom) :
            print "Ecriture du message..."
            # On met le mutex à True
            File.mutex.setMutex(self.nom, True)
            # On va à la fin du fichier
            self.fichier.seek(0,2)
            # Pour écrire le message
            self.fichier.write(str(message) + File.pattern)
            # On force l'écriture
            self.fichier.flush()
            # On remet le mutex à False
            File.mutex.setMutex(self.nom, False)
            
    def close(self) :
        self.fichier.close()
        