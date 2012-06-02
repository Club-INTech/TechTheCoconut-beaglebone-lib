# -*- coding:utf-8 -*-

# Convention : Mutex à 1 <-> PAS POSSIBLE D'ECRIRE.

from time import clock

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
            File.timeout = 1 #second
        
        self.nom = nom
        
        # Ouverture du fichier
        try :
            dejaOuvert = File.mutex.addMutex(self.nom)
            if (not dejaOuvert) and deleteAll: 
                self.fichier = open(self.nom, "w+")
                print "Opening in Delete Mode"
            else :
                self.fichier = open(self.nom, "a+")
            
        except :
            print "Impossible d'ouvrir le fichier " + nom + "."
            
        
    # Retourne True si le message a bien été écrit.
    def write(self, message, noRepetition = True) :
        
        t0 = clock()
        while (clock() <= t0 + File.timeout) :
            # On checke le mutex
            if not File.mutex.getMutex(self.nom) :
                
                # On met le mutex à True
                File.mutex.setMutex(self.nom, True)
                self.fichier.seek(0)
                lines = self.fichier.readlines()
                
                if ((not (message + File.pattern) in lines) and noRepetition) or not noRepetition : 
                
                    print "Ecriture du message..."
                    # On va à la fin du fichier
                    self.fichier.seek(0,2)
                    # Pour écrire le message
                    self.fichier.write(str(message) + File.pattern)
                    # On force l'écriture
                    self.fichier.flush()
                # On remet le mutex à False
                File.mutex.setMutex(self.nom, False)
                return True
        
        # si le timeout est dépassé :
        print "Timeout dépassé !"
        return False
    
        
    # Permet d'enlever le message "message" du fichier.
    # Retourne True si le message a bien été enlevé du fichier.
    def remove(self, message) :
        linesToDelete = []
        t0 = clock()
        
        while clock() <= t0 + File.timeout :
            # On checke le Mutex: 
            if not File.mutex.getMutex(self.nom) :
                File.mutex.setMutex(self.nom, True)
                # On remonte au début du fichier
                self.fichier.seek(0)
                
                lines = self.fichier.readlines()
                # On cherche les lignes où apparaît le message :
                for i in range(len(lines)) :
                    if lines[i] == (message + "\n") :
                        linesToDelete.append(i - len(linesToDelete))
                        
                # on détruit ces lignes :
                for i in range(len(linesToDelete)) :
                    print "Destruction de la ligne n°"+str(linesToDelete[i])
                    del lines[linesToDelete[i]]
                    print lines
                    
                # On réécrit le fichier : destruction
                self.fichier.seek(0)
                self.fichier.truncate(0)
                
                # Puis réécriture
                self.fichier.writelines(lines)
                self.fichier.flush()
                    
                File.mutex.setMutex(self.nom, False)
                return True
                
        print "Timeout dépassé !"
        return False
            
            
    def close(self) :
        self.fichier.close()
        