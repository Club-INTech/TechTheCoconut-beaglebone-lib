# -*- coding:utf-8 -*-

from threading import Lock

# Classe de mutex de file
# Cette classe contient un dictionnaire de mutex, un pour chaque
# fichier ouvert.
class MutexFile :
    def __init__(self):
        # Dictionnaire type {"nomFichier" : Mutex}
        if not hasattr(MutexFile, "mutex") :
            MutexFile.mutex = {}
            
    def __str__(self) :
        return str(MutexFile.mutex)
        
    # Retourne True si le mutex pour ce fichier existait déjà, False sinon
    def addMutex(self, nom) :
        # Ajout du mutex s'il n'existe pas déjà.
        if not nom in MutexFile.mutex :
            lock = Lock()
            MutexFile.mutex.update({nom:lock})
            return False
        return True
    

    def acquire(self, nom) :
        MutexFile.mutex[nom].acquire()
    def release(self, nom) :
        MutexFile.mutex[nom].release()

#--------------------------------------------------------------------------------#
#-----------------------------    CLASSE File    --------------------------------#
#--------------------------------------------------------------------------------#
'''
    Auteur  : Thibaut REMY aka. ~MissFrance~
    Date    : 02 juin 2012
    
    ATTRIBUTS :
    
         ---------------------------------------------- D'instance --------------------------------------------------
        
                self.nom        : nom du fichier à ouvrir.
                self.fichier    : objet de type file.
                
        ------------------------------------------------ De classe --------------------------------------------------
        
                File.mutex      : Instanciation de la classe MutexFile
                File.pattern    : "\n" ou autre. Pattern de fin de message.

    __________________________________________________________________________________________________________________
    
    MÉTHODES :
    
        ------------------------------------------- Fonctions haut niveau -------------------------------------------
        __init__ ( nom ; deleteAll = True) :
                Méthode appellée à l'initialisation du fichier File.
                Cette méthode ouvre aussi le fichier en mode "w+" ou "a+" (w+ si le fichier doit être écrasé)
                
                nom         : nom du fichier.
                deleteAll   : mettre à True si le contenu doit être effacé lors de la première instanciation de ce fichier.
                              mettre à False sinon.
                              
        write   ( message ; noRepetition = True) :
                Méthode utilisée pour écrire une ligne dans un fichier (à la fin de celui-ci)
                Cette méthode utilise un mutex (si son mutex n'est pas accessible, la fonction est bloquante jusqu'à ce qu'il
                       soit accessible)
                       
                message     : Message à écrire (sans \n please)
                noRepetition: Lorsque mis à True, le message ne sera pas écrit si il existe déjà dans le fichier.
                
        remove  (message) :
                Méthode utilisée pour enlever une ou plusieurs lignes d'un fichier.
                Cette méthode utilise un mutex (si son mutex n'est pas accessible, la fonction est bloquante jusqu'à ce qu'il
                       soit accessible)
                       
                message : ligne(s) à enlever du fichier.
                
        clean () :
                Cette méthode enlève les doublons (i.e. les lignes en double) ainsi que les lignes vides dans le fichier.
                Cette méthode utilise un mutex (si son mutex n'est pas accessible, la fonction est bloquante jusqu'à ce qu'il
                       soit accessible)
                 
        close () :
                Ferme définitivement le fichier.
                
        ------------------------------------------------ Fonctions bas-niveau ---------------------------------------------
        
        readlines () :
                Méthode utilisée pour récupérer chaque lignes du fichier.
                /!\ Cette méthode n'utilise pas de mutex. Elle n'est donc pas sensée être utilisée seule.
                
                RETOURNE : Liste de string qui composent les lignes du fichier (sans les '\n')
                
        writelines (lines) :
                Méthode utilisée pour écrire des lignes dans le fichier. Le fichier sera écrasé.
                /!\ Cette méthode n'utilise pas de mutex. Elle n'est donc pas sensée être utilisée seule.
                
                lines   : liste de string qui composeront les lignes du fichier (sans les "\n")
                
                

'''
class File :
    # L'argument deleteAll détermine si oui ou non on veut supprimer les données
    # contenues dans le fichier avant utilisation. (ne peut se faire que pour la
    # première instanciation de ce fichier).
    def __init__(self, nom, deleteAll = True) :
        if not hasattr(File, "mutex") :
            File.mutex = MutexFile()
            File.pattern = "\n"
        
        self.nom = nom
        
        # Ouverture du fichier
        try :
            dejaOuvert = File.mutex.addMutex(self.nom)
            if (not dejaOuvert) and deleteAll: 
                self.fichier = open(self.nom, "w+")
            else :
                self.fichier = open(self.nom, "a+")
            
        except :
            print "Impossible d'ouvrir le fichier " + nom + "."
            
        
    # Retourne True si le message a bien été écrit.
    def write(self, message, noRepetition = True) :
        
        File.mutex.acquire(self.nom)
        
        lines = self.readlines()
        
        if ((not message in lines) and noRepetition) or not noRepetition : 
        
            print "Ecriture du message..."
            # On va à la fin du fichier
            self.fichier.seek(0,2)
            # Pour écrire le message
            self.fichier.write(str(message) + File.pattern)
            # On force l'écriture
            self.fichier.flush()
        
        # On rend le mutex
        File.mutex.release(self.nom)

        
    # WARNING cette fonction ne checke pas le mutex. Elle doit donc être
    # placés à l'intérieur d'un bloc sous l'influence du mutex
    def readlines(self) :
        self.fichier.seek(0)
        lines = self.fichier.readlines()
        
        for i in range(len(lines)) :
            if lines[i][-1] == File.pattern :
                lines[i] = lines[i][0:len(lines[i])-1]
        return lines

    # WARNING cette fonction ne checke pas le mutex. Elle doit donc être
    # placés à l'intérieur d'un bloc sous l'influence du mutex
    def writelines(self, lines) :
        for i in range(len(lines)) :
            lines[i] = lines[i] + File.pattern
        
        self.fichier.seek(0)
        self.fichier.truncate(0)
        self.fichier.writelines(lines)
        self.fichier.flush()


        
    # Permet d'enlever le message "message" du fichier.
    # Retourne True si le message a bien été enlevé du fichier.
    def remove(self, message) :
        
        linesToDelete = []
        File.mutex.acquire(self.nom)
        
        lines = self.readlines()
        
        # On cherche les lignes où apparaît le message :
        for i in range(len(lines)) :
            if lines[i] == message :
                linesToDelete.append(i - len(linesToDelete))
                
        # on détruit ces lignes :
        for i in range(len(linesToDelete)) :
            del lines[linesToDelete[i]]
            
        self.writelines(lines)
        
        File.mutex.release(self.nom)

        
    # Proprifie le fichier (i.e. enlève les doublons et les lignes vides)
    def clean(self) :
        # Contient l'indices de lignes à détruire
        linesToDelete = []
        # Contient les lignes du fichier mais sans les doublons
        linesDejaVu   = []
        
        File.mutex.acquire(self.nom)

        lines = self.readlines()
        
        for i in range(len(lines)) :
            if (not lines[i] in linesDejaVu) and lines[i] != "" :
                linesDejaVu.append(lines[i])
                
        self.writelines(linesDejaVu)
        
        File.mutex.release(self.nom)

        
            
            
    def close(self) :
        self.fichier.close()
        