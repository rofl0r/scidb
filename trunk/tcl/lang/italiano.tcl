# ======================================================================
# Author : $Author$
# Version: $Revision: 1527 $
# Date   : $Date: 2018-10-26 12:11:06 +0000 (Fri, 26 Oct 2018) $
# Url    : $URL$
# ======================================================================

# ======================================================================
#    _/|            __
#   // o\         /    )           ,        /    /
#   || ._)    ----\---------__----------__-/----/__-
#   //__\          \      /   '  /    /   /    /   )
#   )___(     _(____/____(___ __/____(___/____(___/_
# ======================================================================

# ======================================================================
# Copyright: (C) 2011-2013 Gregor Cramer
# Copyright: (C) 2011-2012 Giovanni Ornaghi
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

# ======================================================================
# File encoding: utf-8
# ======================================================================

### global #############################################################
::mc::SortMapping		{Å A å a à a À A è e È E é e É E ì i Ì i ò o Ò O ù u Ù U}
::mc::AsciiMapping	{Å A à a à a À A è e È E é e É E ì i Ì i ò o Ò O ù u Ù U}
::mc::SortOrder		{}


::mc::Key(Alt)				"Alt"
::mc::Key(BS)				"\u27fb" ;# "\u232b" is correct, but difficult to read
::mc::Key(Ctrl)			"Ctrl"
::mc::Key(Del)				"Canc"
::mc::Key(Down)			"\u2193"
::mc::Key(End)				"Fine"
::mc::Key(Enter)			"Invio"
::mc::Key(Esc)				"Esci"
::mc::Key(Home)			"Inizio" ;# "Home"
::mc::Key(Ins)				"Ins"
::mc::Key(Left)			"\u2190"
::mc::Key(Next)			"Page\u2193"
::mc::Key(Option)			"option" ;# Mac
::mc::Key(Prior)			"Page\u2191"
::mc::Key(Right)			"\u2192"
::mc::Key(Shift)			"Shift"
::mc::Key(Space)			"\u2423"
::mc::Key(Up)				"\u2191"

::mc::KeyDescr(Space)	"Space" ;# NEW

::mc::Alignment			"Allineamento"
::mc::Apply					"Applica"
::mc::Archive				"Archivio"
::mc::Background			"Sfondo"
::mc::Black					"Nero"
::mc::Bottom				"Basso"
::mc::Cancel				"Annulla"
::mc::Clear					"Pulisci"
::mc::Close					"Chiudi"
::mc::Color					"Colore"
::mc::Colors				"Colori"
::mc::Configuration		"Configurazione"
::mc::Copy					"Copia"
::mc::Country				"Country" ;# NEW
::mc::Cut					"Taglia"
::mc::Dark					"Scura"
::mc::Database				"Database"
::mc::Default				"Default"
::mc::Delete				"Elimina"
::mc::Edit					"Modifica"
::mc::Empty					"Vuoto"
::mc::Enabled				"Enabled" ;# NEW
::mc::Error					"Error" ;# NEW
::mc::File					"File"
::mc::Filter				"Filtro"
::mc::From					"Da"
::mc::Game					"Partita"
::mc::Hidden				"Hidden" ;# NEW
::mc::InternalMessage	"Internal message" ;# NEW
::mc::Layout				"Layout"
::mc::Left					"Sinistra"
::mc::Lite					"Chiara"
::mc::Low					"Basso"
::mc::Modify				"Cambia"
::mc::Monospaced			"Monospaced" ;# NEW monospaced font
::mc::No						"no"
::mc::Normal				"Normale"
::mc::Number				"Numero"
::mc::OK						"OK"
::mc::Order					"Ordine"
::mc::Page					"Page" ;#NEW
::mc::Paste					"Incolla"
::mc::PieceSet				"Set di Pezzi"
::mc::Preview				"Anteprima"
::mc::Redo					"Rifai"
::mc::Remove				"Rimuovi"
::mc::Reset					"Reset"
::mc::Right					"Destra"
::mc::SelectAll			"Seleziona Tutto"
::mc::Tab					"Tab" ;# NEW
::mc::Texture				"Texture"
::mc::Theme					"Tema"
::mc::To						"A"
::mc::Top					"Alto"
::mc::Undo					"Indietro"
::mc::Unknown				"Unknown" ;# NEW
::mc::Variant				"Variante Eterodossa"
::mc::Variation			"Variante"
::mc::Volume				"Volume" ;# NEW
::mc::White					"Bianco"
::mc::Yes					"sì"

::mc::Piece(K)				"Re"
::mc::Piece(Q)				"Donna"
::mc::Piece(R)				"Torre"
::mc::Piece(B)				"Alfiere"
::mc::Piece(N)				"Cavallo"
::mc::Piece(P)				"Pedone"

::mc::PieceCQL(.)			"Empty square" ;# NEW
::mc::PieceCQL(A)			"Any white piece" ;# NEW
::mc::PieceCQL(a)			"Any black piece" ;# NEW
::mc::PieceCQL(M)			"White major piece" ;# NEW
::mc::PieceCQL(m)			"Black major piece" ;# NEW
::mc::PieceCQL(I)			"White minor piece" ;# NEW
::mc::PieceCQL(i)			"Black minor piece" ;# NEW
::mc::PieceCQL(U)			"Any piece at all" ;# NEW
::mc::PieceCQL(?)			"Any piece or empty square" ;# NEW

::mc::SquareCQL(L)		"Light squares" ;# NEW
::mc::SquareCQL(D)		"Dark squares" ;# NEW

::mc::SquareLetter(L)	"L" ;# NEW shortcut for 'light square' (uppercase)
::mc::SquareLetter(D)	"D" ;# NEW shortcut for 'dark square'  (uppercase)

::mc::Logical(reset)		"Reset"
::mc::Logical(or)			"OR"
::mc::Logical(and)		"AND"
::mc::Logical(null)		"Nessuno"
::mc::Logical(remove)	"Rimuovi"
::mc::Logical(not)		"NOT"

::mc::LogicalDetail(reset)			"Annulla filtro / Reset visualizzazione"
::mc::LogicalDetail(or)				"Rimuovi dal filtro / Aggiungi alla visualizzazione"
::mc::LogicalDetail(and)			"Estendi filtro / Restringi visualizzazione"
::mc::LogicalDetail(null)			"Riempi filtro / Pulisci visualizzazione"
::mc::LogicalDetail(remove)		"Aggiunti al filtro / Rimuovi dalla visualizzazione"
::mc::LogicalDetail(not)			"Registringi filtro / Estendi visualizzazione"

::mc::VariantName(Undetermined)	"Indeterminato"
::mc::VariantName(Normal)			"Scacchi Ortodossi"
::mc::VariantName(Bughouse)		"Scacchi Mangia-passa"
::mc::VariantName(DropChess)		"Drop Chess" ;# NEW this is the main term for Crazyhouse and Chessgi
::mc::VariantName(Crazyhouse)		"Scacchi Crazyhouse"
::mc::VariantName(Chessgi)			"Chessgi" ;# NEW
::mc::VariantName(ThreeCheck)		"Tre Scacchi"
::mc::VariantName(Antichess)		"Vinciperdi" ;# this is the main term for Suicide and Giveaway
::mc::VariantName(Suicide)			"Vinciperdi" ;# NEW seems to be wrong
::mc::VariantName(Giveaway)		"Giveaway"
::mc::VariantName(Losers)			"Vinciperdi" ;# NEW seems to be wrong
::mc::VariantName(Chess960)		"Scacchi 960"
::mc::VariantName(Symm960)			"Scacchi 960 (solo simmetrica)"
::mc::VariantName(Shuffle)			"Scacchi Shuffle"

### themes #############################################################
::scidb::themes::mc::CannotOverwriteTheme	"Impossibile sovrascrivere il tema %s."

### file ###############################################################
::file::mc::CheckPermissions	"Check file permissions." ;# NEW
::file::mc::NotAvailable		"Either this file is not available anymore, or the file permissions are not allowing access." ;# NEW

::file::mc::DoesNotExist(readable)		"File '%s' is not readable." ;# NEW
::file::mc::DoesNotExist(writable)		"File '%s' is not writable." ;# NEW
::file::mc::DoesNotExist(executable)	"File '%s' is not executable." ;# NEW

### locale #############################################################
::locale::Pattern(decimalPoint)	"."
::locale::Pattern(thousandsSep)	","
::locale::Pattern(dateY)			"Y"
::locale::Pattern(dateM)			"M Y"
::locale::Pattern(dateD)			"D M, Y"
::locale::Pattern(time)				"D M, Y, h:m"
::locale::Pattern(normal:dateY)	"Y"
::locale::Pattern(normal:dateM)	"M/Y"
::locale::Pattern(normal:dateD)	"D/M/Y"

### widget #############################################################
::widget::mc::Label(apply)			"&Applica"
::widget::mc::Label(cancel)		"&Cancella"
::widget::mc::Label(clear)			"P&ulisci"
::widget::mc::Label(close)			"C&hiudi"
::widget::mc::Label(ok)				"&OK"
::widget::mc::Label(reset)			"&Reset"
::widget::mc::Label(update)		"A&ggiorna"
::widget::mc::Label(import)		"&Importa"
::widget::mc::Label(revert)		"In&verti"
::widget::mc::Label(previous)		"Prece&dente"
::widget::mc::Label(next)			"Pro&ssima"
::widget::mc::Label(first)			"Pri&ma"
::widget::mc::Label(last)			"&Ultima"
::widget::mc::Label(help)			"&Aiuto"
::widget::mc::Label(start)			"&Inizia"
::widget::mc::Label(new)			"&Nuovo"
::widget::mc::Label(save)			"&Salva"
::widget::mc::Label(delete)		"&Elimina"

::widget::mc::Control(minimize)	"Minimizza"
::widget::mc::Control(restore)	"Esci da Schermo-Interno"
::widget::mc::Control(close)		"Chiudi"

### util ###############################################################

::util::mc::IOErrorOccurred					"Errore Input/Output"

::util::mc::IOError(CreateFailed)			"permessi di creazione file negati"
::util::mc::IOError(OpenFailed)				"apertura fallita"
::util::mc::IOError(ReadOnly)					"il database è in sola-lettura"
::util::mc::IOError(UnknownVersion)			"versione del file sconosciuta"
::util::mc::IOError(UnexpectedVersion)		"versione del file inattesa"
::util::mc::IOError(Corrupted)				"file corrotto"
::util::mc::IOError(WriteFailed)				"operazione di scrittura fallita"
::util::mc::IOError(InvalidData)				"dato non valido (possibile corruzione file)"
::util::mc::IOError(ReadError)				"errore di lettura"
::util::mc::IOError(EncodingFailed)			"non posso scrivere il file di namebase"
::util::mc::IOError(MaxFileSizeExceeded)	"grandezza di file massima raggiunta"
::util::mc::IOError(LoadFailed)				"apertura fallita (troppi eventi inseriti)"
::util::mc::IOError(NotOriginalVersion)	"dall'ultima apertura i file sono stati modificati al di fuori di questa sessione"
::util::mc::IOError(CannotCreateThread)	"cannot create thread (low memory?)" ;# NEW

::util::mc::SelectionOwnerDidntRespond		"Tempo scaduto durante operazione di trascinamento: intestatario selezione non ha risposto."

### progress ###########################################################
::progress::mc::Progress							"Progressi"

::progress::mc::Message(preload-namebase)		"Pre-caricando dati di namebase"
::progress::mc::Message(preload-tournament)	"Pre-caricando indice tornei"
::progress::mc::Message(preload-player)		"Pre-caricando indice giocatori"
::progress::mc::Message(preload-annotator)	"Pre-caricando indice commentatori"

::progress::mc::Message(read-index)				"Caricando indice dati"
::progress::mc::Message(read-game)				"Caricando dati partite"
::progress::mc::Message(read-namebase)			"Caricando dati namebase"
::progress::mc::Message(read-tournament)		"Caricando indice tornei"
::progress::mc::Message(read-player)			"Caricando indice giocatori"
::progress::mc::Message(read-annotator)		"Caricando indice commentatori"
::progress::mc::Message(read-source)			"Caricando indice fonti"
::progress::mc::Message(read-team)				"Caricando indice squadre"
::progress::mc::Message(read-init)				"Caricando dati di inizializzazione"

::progress::mc::Message(write-index)			"Scrivendo indice dati"
::progress::mc::Message(write-game)				"Scrivendo dati partite"
::progress::mc::Message(write-namebase)		"Scrivendo dati namebase"

::progress::mc::Message(print-game)				"Stampa %s partite"
::progress::mc::Message(copy-game)				"Copia %s partite"

### menu ###############################################################
::menu::mc::Theme								"Tema"
::menu::mc::ColorScheme						"Schema Colori"
::menu::mc::CustomStyleMenu				"Stile Menu di Scidb"
::menu::mc::DefaultStyleMenu				"Stile Menu di Default"
::menu::mc::OrdinaryMonitor				"Monitor Normale"
::menu::mc::HighQualityMonitor			"Monitor di Alta Qualità"
::menu::mc::RestartRequired				"Il riavvio dell'applicazione è necessario prima che sia possibile applicare le modifiche"

::menu::mc::AllScidbFiles					"Tutti i file Scidb"
::menu::mc::AllScidbBases					"Tutti i Database Scidb"
::menu::mc::ScidBases						"Database Scid"
::menu::mc::ScidbBases						"Database Scidb"
::menu::mc::ChessBaseBases					"Database ChessBase"
::menu::mc::ScidbArchives					"Archivi Scidb"
::menu::mc::PGNFilesArchives				"File/Archivi PGN"
::menu::mc::PGNFiles							"File PGN"
::menu::mc::PGNFilesCompressed			"File PGN (compresso)"
::menu::mc::BPGNFilesArchives				"File/Archivi BPGN"
::menu::mc::BPGNFiles						"File BPGN"
::menu::mc::BPGNFilesCompressed			"File BPGN (compresso)"
::menu::mc::PGNArchives						"Archivi PGN"

::menu::mc::Language							"&Lingua"
::menu::mc::Toolbars							"&Barre strumenti"
::menu::mc::ShowLog							"Mostra &Log"
::menu::mc::AboutScidb						"S&u Scidb"
::menu::mc::TipOfTheDay						"Tip of the &Day" ;# NEW
::menu::mc::Fullscreen						"&Schermo intero"
::menu::mc::LeaveFullscreen				"Esci da &Schermo intero"
::menu::mc::Help								"&Aiuto"
::menu::mc::Contact							"&Contenuti (browser)"
::menu::mc::Quit								"&Esci"
::menu::mc::Tools								"&Strumenti"
::menu::mc::Extras							"E&xtras"
::menu::mc::Setup								"Impos&ta"
::menu::mc::Layout							"La&yout" ;# NEW

# Font Size
::menu::mc::IncrFontSize					"Increase All Font Sizes" ;# NEW
::menu::mc::DecrFontSize					"Decrease All Font Sizes" ;# NEW

# Contact
::menu::mc::ContactBugReport				"&Segnala Bug"
::menu::mc::ContactFeatureRequest		"&Richiesta Funzione"

# Extras
::menu::mc::InstallChessBaseFonts		"Installa Caratteri ChessBase"
::menu::mc::OpenEngineLog					"Apri Console &Motori"
::menu::mc::AssignFileTypes				"Assign File &Types" ;# NEW

# Tools
::menu::mc::OpenEngineDictionary			"Apri &Dizionario Motore"
::menu::mc::OpenPlayerDictionary			"Apri Dizionario &Giocatore"

# Setup
::menu::mc::Engines							"&Motori"
::menu::mc::PgnOptions						"Imposta opzioni di esportazione &PGN"
::menu::mc::PrivatePlayerCard				"Profilo &Privato del Giocatore"

::menu::mc::OpenFile							"Apri un file Scidb"
::menu::mc::NewFile							"Crea un file Scidb"
::menu::mc::Archiving						"Archiviazione"
::menu::mc::CreateArchive					"Crea Archivio"
::menu::mc::BuildArchive					"Crea Archivio %s"
::menu::mc::Data								"%s dati"

# Default Application
::menu::mc::Assign							"assegna"
::menu::mc::FailedSettingDefaultApp		"Tentativo di impostare Scidb come applicazione di default fallito per %s."
::menu::mc::SuccessSettingDefaultApp	"Impostato Scidb come applicazione di default per %s con successo"
::menu::mc::CommandFailed					"Comando '%s' fallito"

### load ###############################################################
::load::mc::SevereError				"Grave errore durante caricamento file ECO"
::load::mc::FileIsCorrupt			"Il file %s è corrotto:"
::load::mc::ProgramAborting		"Il programma si sta chiudendo."
::load::mc::EngineSetupFailed		"Caricamento della configurazione del motore fallita"

::load::mc::Loading					"Carico %s"
::load::mc::StartupFinished		"Startup completato"
::load::mc::SystemEncoding			"La codifica di sistema è '%s'"
::load::mc::Startup					"Startup" ;# NEW

::load::mc::ReadingFile(options)	"Permessi di lettura del file"
::load::mc::ReadingFile(engines)	"Lettura file dei motori"

::load::mc::ECOFile					"File ECO"
::load::mc::EngineFile				"File motore"
::load::mc::SpellcheckFile			"File di informazioni sul giocatore"
::load::mc::LocalizationFile		"File localizzazione"
::load::mc::RatingList				"%s lista punteggio"
::load::mc::WikipediaLinks			"Link a Wikipedia"
::load::mc::ChessgamesComLinks	"Link a chessgames.com"
::load::mc::Cities					"città"
::load::mc::PieceSet					"set di pezzi"
::load::mc::Theme						"temi"
::load::mc::Icons						"icone"

### archive ############################################################
::archive::mc::CorruptedArchive			"L'archivio '%s' è corrotto."
::archive::mc::NotAnArchive				"'%s' non è un archivio."
::archive::mc::CorruptedHeader			"L'intestazione dell'archivio in '%s' è corrotta."
::archive::mc::CannotCreateFile			"Fallita creazione del file '%s'."
::archive::mc::FailedToExtractFile		"Fallita estrazione del file '%s'."
::archive::mc::UnknownCompression		"Metodo di compressione sconosciuto '%s'."
::archive::mc::ChecksumError				"Errore nel checksum durante l'estrazione '%s'."
::archive::mc::ChecksumErrorDetail		"Il file estratto '%s' sarà corrotto."
::archive::mc::FileNotReadable			"Il file '%s' non è leggibile."
::archive::mc::UsingRawInstead			"Utilizzo il metodo di compressione 'raw'."
::archive::mc::CannotOpenArchive			"Impossibile aprire l'archivio '%s'."
::archive::mc::CouldNotCreateArchive	"Impossibile creare l'archivio '%s'."

::archive::mc::PackFile						"Comprimi %s"
::archive::mc::UnpackFile					"Estrai %s"

### player photos ######################################################
::util::photos::mc::InstallPlayerPhotos		"Installa/Aggiorna le foto dei giocatori"
::util::photos::mc::TimeOut						"Tempo per la connessione scaduto."
::util::photos::mc::EnterPassword				"Password personale"
::util::photos::mc::Download						"Download"
::util::photos::mc::SharedInstallation			"Installazione condivisa"
::util::photos::mc::LocalInstallation			"Installazione privata"
::util::photos::mc::RetryLater					"Per favore riprova più tardi."
::util::photos::mc::DownloadStillInProgress	"Il download delle foto è ancora in corso."
::util::photos::mc::PhotoFiles					"File di foto del giocatore"

::util::photos::mc::RequiresSuperuserRights	"L'installazione/aggiornamento richiede diritti di super-user.\n\nNota che l apassword non sarà accettata se il tuo utente non è nel gruppo wheel." ;# sudoers file?
::util::photos::mc::RequiresInternetAccess	"L'installazione/aggiornamento delle foto dei giocatori richiede una connessione internet."
::util::photos::mc::AlternativelyDownload(0)	"Alternativamente puoi scaricare le foto da %link%. Installa questi file nella directory %local%."
::util::photos::mc::AlternativelyDownload(1)	"Alternativamente puoi scaricare questi file da %link%. Installa questi file nella cartella condivisa %shared%, o nella cartella privata %local%."

::util::photos::mc::Error(nohttp)				"Non posso aprire una connessione internet perché il pacchetto TclHttp non è installato."
::util::photos::mc::Error(busy)					"L'installazione/aggiornamento è ancora in corso."
::util::photos::mc::Error(failed)				"Errore inaspettato: l'invocazione della sub-routine è fallita."
::util::photos::mc::Error(passwd)				"La password è sbagliata."
::util::photos::mc::Error(nosudo)				"Impossibile invocare il comando 'sudo' perché il tuo utente non è nel gruppo wheel."
::util::photos::mc::Detail(nosudo)				"Per aggirare il problema puoi fare un'installazione privata, o lanciare il programma come super-user."

::util::photos::mc::Message(uptodate)			"Le foto sono già aggiornate."
::util::photos::mc::Message(finished)			"L'installazione/aggiornamento delle foto è finito."
::util::photos::mc::Message(broken)				"La versione della libreria Tcl è corrotta."
::util::photos::mc::Message(noperm)				"Non hai diritti di scittura nella cartella '%s'."
::util::photos::mc::Message(missing)			"Impossibile trovare la cartella '%s'."
::util::photos::mc::Message(httperr)			"Errore HTTP: %s"
::util::photos::mc::Message(httpcode)			"Codice HTTP inaspettato %s."
::util::photos::mc::Message(noconnect)			"Connessione HTTP fallita."
::util::photos::mc::Message(timeout)			"Tempo di connessione HTTP scaduto."
::util::photos::mc::Message(crcerror)			"Errore nel Checksum. Probabilmente il server è al momento sottoposto a manutenzione."
::util::photos::mc::Message(maintenance)		"Il server che ospita le foto è al momento in manutenzione."
::util::photos::mc::Message(notfound)			"Download interrotto perché il server che ospita le foto è al momento in manutenzione."
::util::photos::mc::Message(noreply)			"Server is not replying." ;# NEW
::util::photos::mc::Message(aborted)			"L'utente ha interrotto il download."
::util::photos::mc::Message(killed)				"Interruzione inaspettata del download. La sub-routine è cessata."

::util::photos::mc::Detail(nohttp)				"Per favore installa il pacchetto TclHttp, per esemio %s."
::util::photos::mc::Detail(noconnect)			"Probabilmente non hai una connessione internet."
::util::photos::mc::Detail(badhost)				"Un'altra possibilità è un cattivo host, o una cattiva porta."

::util::photos::mc::Log(started)					"L'installazione/aggiornamento delle foto è cominciato a %s."
::util::photos::mc::Log(finished)				"L'installazione/aggiornamneto delle foto è finito a %s."
::util::photos::mc::Log(destination)			"La cartella di destinazione del download è '%s'."
::util::photos::mc::Log(created:1)				"%s file creati."
::util::photos::mc::Log(created:N)				"%s file(s) creati."
::util::photos::mc::Log(deleted:1)				"%s file eliminati."
::util::photos::mc::Log(deleted:N)				"%s file(s) eliminati."
::util::photos::mc::Log(skipped:1)				"%s file saltati."
::util::photos::mc::Log(skipped:N)				"%s file(s) saltati."
::util::photos::mc::Log(updated:1)				"%s file aggiornati."
::util::photos::mc::Log(updated:N)				"%s file(s) aggiornati."

### tip of the day #####################################################
::tips::mc::TipOfTheDay				"Tip of the Day" ;# NEW
::tips::mc::FurtherInformation	"Further information" ;# NEW
::tips::mc::CouldNotOpenFile		"Could not open file %s." ;# NEW
::tips::mc::CouldNotFindAnyTip	"Could not find any tip." ;# NEW
::tips::mc::RepeatAllTips			"Repeat all tips (restart from the beginning)" ;# NEW
::tips::mc::NextTip					"Next Tip" ;# NEW
::tips::mc::FirstTip					"<p>The Tip-of-the-Day information serves to a better insight into the functioning of this application. Furthermore it will give useful hints that will help to know what is possible.</p><p color='darkgreen'><b>Have joy with Scidb!</b></p>" ;# NEW

::tips::mc::Choice(everytime)					"Show everytime" ;# NEW
::tips::mc::Choice(periodically)				"Show periodically" ;# NEW
::tips::mc::Choice(everytimeWhenNew)		"Show everytime, but only new tips" ;# NEW
::tips::mc::Choice(periodicallyWhenNew)	"Show periodically, but only new tips" ;# NEW
::tips::mc::Choice(neverShow)					"Don't show anymore" ;# NEW

### twm - tiled window manager #########################################
::twm::mc::Close				"Chiudi"
::twm::mc::Undock				"Sblocca"
::twm::mc::Amalgamate		"Amalgamate" ;# NEW
::twm::mc::Separate			"Separate" ;# NEW

::twm::mc::Timeout			"Timeout after eight seconds without mouse motions, the frame has been re-docked to old place." ;# NEW
::twm::mc::TimeoutDetail	"This safety handling is required to avoid frozen screens, as long as the tiling window management is in an experimental stage." ;# NEW

### application::layout ################################################
::application::layout::mc::Rename				"Rename" ;# NEW
::application::layout::mc::Delete				"Delete" ;# NEW
::application::layout::mc::Replace				"Replace" ;# NEW
::application::layout::mc::Load					"Load" ;# NEW
::application::layout::mc::Linked				"Linked with" ;# NEW
::application::layout::mc::CannotOpenFile		"Cannot read file '%s'." ;# NEW
::application::layout::mc::RestoreOldLayout	"Restore to old layout" ;# NEW
::application::layout::mc::ReplaceTip			"Overwrite current layout with this one" ;# NEW

### application ########################################################
::application::mc::Tab(information)			"&Information" ;# NEW
::application::mc::Tab(database)				"&Database"
::application::mc::Tab(board)					"&Scacchiera"
::application::mc::Tab(games)					"&Partite"
::application::mc::Tab(player)				"&Giocatori"
::application::mc::Tab(event)					"&Eventi"
::application::mc::Tab(site)					"&Luoghi"
::application::mc::Tab(position)				"S&tart Positions"
::application::mc::Tab(annotator)			"&Commentatori"
::application::mc::MainMenu					"&Menù principale"

::application::mc::ChessInfoDatabase		"Chess Information Data Base"
::application::mc::Shutdown					"Spegni..."
::application::mc::QuitAnyway					"Uscire comunque?"
::application::mc::CancelLogout				"Annulla Logout"
::application::mc::AbortWriteOperation		"Annulla operazione di scrittura"
::application::mc::UpdatesAvailable			"Aggiornamenti disponibili"

::application::mc::WriteOperationInProgress "Operazione di scrittura in corso: al momento Scidb sta modificando\scrivendo il database '%s'."
::application::mc::LogoutNotPossible		"Il logout non è possibile al momento, il risultato sarebbe un database corrotto."
::application::mc::RestartLogout				"Annullare l'operazione di scrittura riavvierà il processo di logout."
::application::mc::UnsavedFiles				"I seguenti file PGN non sono salvati:"
::application::mc::ThrowAwayAllChanges		"Vuoi davvero scartare tutti i cambiamenti?"

::application::mc::Deleted						"Partite eliminate: %d"
::application::mc::Changed						"Partite modificate: %d"
::application::mc::Added						"Partite aggiunte: %d"
::application::mc::DescriptionHasChanged	"La descrizione è stata modificata"

### application::twm ###################################################
::application::twm::mc::Notebook						"Notebook" ;# NEW
::application::twm::mc::Multiwindow					"Stack" ;# NEW
::application::twm::mc::FoldTitleBar				"Fold Titlebar" ;# NEW
::application::twm::mc::FoldAllTitleBars			"Fold all Titlebars" ;# NEW
::application::twm::mc::UnfoldAllTitleBars		"Unfold all Titlebars" ;# NEW
::application::twm::mc::AmalgamateTitleBar		"Amalgamate Titlebar" ;# NEW
::application::twm::mc::AmalgamateAllTitleBars	"Amalgamate all Titlebars" ;# NEW
::application::twm::mc::SeparateAllTitleBars		"Separate all Titlebars" ;# NEW
::application::twm::mc::AlignToLine					"Align to Line Space" ;# NEW
::application::twm::mc::MoveWindow					"Move Window" ;# NEW
::application::twm::mc::StayOnTop					"Stay on Top" ;# NEW
::application::twm::mc::HideWhenLeavingTab		"Hide When Leaving Tab" ;# NEW
::application::twm::mc::SaveLayout					"Save Layout" ;# NEW
::application::twm::mc::SaveLayoutAs				"Save Layout as %s" ;# NEW
::application::twm::mc::RenameLayout				"Rename Layout" ;# NEW
::application::twm::mc::LoadLayout					"Load Layout" ;# NEW
::application::twm::mc::NewLayout					"New Layout" ;# NEW
::application::twm::mc::ManageLayouts				"Manage Layouts" ;# NEW
::application::twm::mc::ShowAllDockingPoints		"Show all Docking Points" ;# NEW
::application::twm::mc::DockingArrowSize			"Docking Arrow Size" ;# NEW
::application::twm::mc::LinkLayout					"Link Layout '%s'" ;# NEW
::application::twm::mc::UnlinkLayout				"Delete Link to '%s'" ;# NEW
::application::twm::mc::LinkLayoutTip				"Link With Board Layout" ;# NEW
::application::twm::mc::Actual						"current" ;# NEW
::application::twm::mc::Changed						"changed" ;# NEW
::application::twm::mc::Windows						"Windows" ;# NEW
::application::twm::mc::ConfirmDelete				"Really delete layout '%s'?" ;# NEW
::application::twm::mc::ConfirmDeleteDetails		"This will only delete the layout of variant '%s'. If you want to delete the complete layout then you have to delete the layout of variant '%s'." ;# NEW
::application::twm::mc::ConfirmOverwrite			"Overwrite existing layout '%s'?" ;# NEW
::application::twm::mc::LayoutSaved					"Layout '%s' successfully saved." ;# NEW
::application::twm::mc::EnterName					"Enter Name" ;# NEW
::application::twm::mc::UnsavedLayouts				"At least one layout has been changed. Either cancel the termination of the application, or commit the selected actions." ;# NEW
::application::twm::mc::LinkWithLayout				"Link with eponymous board layout '%s'?" ;# NEW
::application::twm::mc::CopyLayoutFrom				"Copy layout from" ;# NEW
::application::twm::mc::ApplyToAllLayouts			"Apply this action to all changed layouts?" ;# NEW
::application::twm::mc::KeepEnginesOpen			"Current layout has more analysis windows than selected layout. Keep all additional analysis windows open?" ;# NEW
::application::twm::mc::ErrorInOptionFile			"Option file for layout variant '%s' is corrupted." ;# NEW

::application::twm::mc::Pane(analysis)		"Analysis" ;# NEW
::application::twm::mc::Pane(board)			"Board" ;# NEW
::application::twm::mc::Pane(editor)		"Notation" ;# NEW
::application::twm::mc::Pane(tree)			"Tree" ;# NEW
::application::twm::mc::Pane(games)			"Games" ;# NEW
::application::twm::mc::Pane(player)		"Players" ;# NEW
::application::twm::mc::Pane(event)			"Events" ;# NEW
::application::twm::mc::Pane(annotator)	"Annotators" ;# NEW
::application::twm::mc::Pane(site)			"Sites" ;# NEW
::application::twm::mc::Pane(position)		"Start Positions" ;# NEW
::application::twm::mc::Pane(eco)			"ECO-Table" ;# NEW

::application::twm::mc::UnsavedAction(discard)		"Discard changes (start next time with unchanged layout)" ;# NEW
::application::twm::mc::UnsavedAction(overwrite)	"Overwrite existing layout with changed layout" ;# NEW
::application::twm::mc::UnsavedAction(disconnect)	"Disconnect from original layout, but retain the changes" ;# NEW
::application::twm::mc::UnsavedAction(retain)		"Retain changes, and do not disconnect" ;# NEW

### application::eco ###################################################
::application::eco::mc::SelectEco		"Select ECO code" ;# NEW

::application::eco::mc::Mode(single)	"Per ply" ;# NEW
::application::eco::mc::Mode(compact)	"Transitions only" ;# NEW

::application::eco::mc::F_Line			"Line" ;# NEW

### application::board #################################################
::application::board::mc::ShowCrosstable				"Mostra tabella torneo per questa partita"
::application::board::mc::StartEngine					"Fai partire il motore di analisi" ;# NEW content: Start chess analysis engine in new window
::application::board::mc::InsertNullMove				"Inserisci mossa nulla"
::application::board::mc::SelectStartPosition		"Seleziona Posizione Iniziale"
::application::board::mc::LoadRandomGame				"Carica partita casualmente"
::application::board::mc::AddNewGame					"Aggiunti nuova partita..."
::application::board::mc::SlidingVarPanePosition	"Sliding variation pane position" ;# NEW
::application::board::mc::ShowVariationArrows		"Show variation arrows" ;# NEW
::application::board::mc::ShowAnnotation           "Show annotation glyph" ;# NEW
::application::board::mc::ShowAnnotationTimeout    "Timeout for annotation glyph" ;# NEW
::application::board::mc::None                     "None" ;# NEW

::application::board::mc::MarkPromotedPiece			"Mark promoted pieces" ;# NEW
::application::board::mc::PromoSign(none)				"None" ;# NEW
::application::board::mc::PromoSign(bullet)			"Bullet" ;# NEW
::application::board::mc::PromoSign(star)				"Star" ;# NEW
::application::board::mc::PromoSign(disk)				"Disk" ;# NEW

::application::board::mc::Tools							"Strumenti"
::application::board::mc::Control						"Controllo"
::application::board::mc::Database						"Database"
::application::board::mc::GoIntoNextVar				"Vai alla prossima variante"
::application::board::mc::GoIntPrevVar					"Vai alla precedente variante"

::application::board::mc::LoadGame(next)				"Carica la prossima partita"
::application::board::mc::LoadGame(prev)				"Carica la partita precedente"
::application::board::mc::LoadGame(first)				"Carica la prima partita"
::application::board::mc::LoadGame(last)				"Carica l'ultima partita"
::application::board::mc::LoadFirstLast(next)		"End of list reached, continue with first game?" ;# NEW
::application::board::mc::LoadFirstLast(prev)		"Start of list reached, continue with last game?" ;# NEW

::application::board::mc::SwitchView(base)			"Passa alla vista database"
::application::board::mc::SwitchView(list)			"Passa alla vista della partita"

::application::board::mc::Accel(edit-annotation)	"A"
::application::board::mc::Accel(edit-comment)		"C"
::application::board::mc::Accel(edit-marks)			"M"
::application::board::mc::Accel(add-new-game)		"S"
::application::board::mc::Accel(replace-game)		"R"
::application::board::mc::Accel(replace-moves)		"V"
::application::board::mc::Accel(trial-mode)			"T"
::application::board::mc::Accel(export-game)			"E"

### application::information ###########################################
::application::information::mc::RecentlyUsed           "Recently used" ;# NEW
::application::information::mc::RemoveSelectedDatabase "Remove selected database from history" ;# NEW
::application::information::mc::NewsAvailable          "There are updatednews available" ;# NEW
::application::information::mc::NoInternetConnection   "Information: Scidb cannot connect to Internet." ;# NEW

### application::database ##############################################
::application::database::mc::FileOpen							"Apri Database"
::application::database::mc::FileOpenRecent					"Apri Recente"
::application::database::mc::FileNew							"Nuovo Database"
::application::database::mc::FileExport						"Esporta"
::application::database::mc::FileImport(pgn)					"Importa file PGN"
::application::database::mc::FileImport(db)					"Importa Database"
::application::database::mc::FileCreate						"Crea Archivio"
::application::database::mc::FileSaveChanges					"Salva Modifiche"
::application::database::mc::FileClose							"Chiudi"
::application::database::mc::FileMaintenance					"Manutenzione"
::application::database::mc::FileCompact						"Compresso"
::application::database::mc::FileStripMoveInfo				"Rimuovi Informazioni Mossa"
::application::database::mc::FileStripPGNTags				"Rimuovi Tag PGN"
::application::database::mc::HelpSwitcher						"Aiuto per il Selezionatore di Database"

::application::database::mc::File								"File"
::application::database::mc::SymbolSize						"Grandezza simbolo"
::application::database::mc::Large								"Grande"
::application::database::mc::Medium								"Medio"
::application::database::mc::Small								"Piccolo"
::application::database::mc::Tiny								"Minuscolo"
::application::database::mc::LoadMessage						"Sto aprendo il database '%s'"
::application::database::mc::UpgradeMessage					"Aggiornamento database '%s'"
::application::database::mc::CompactMessage					"Comprimendo il database '%s'"
::application::database::mc::CannotOpenFile					"Non è possibile aprire il file '%s'."
::application::database::mc::EncodingFailed					"Codifica di %s fallita." ;# "Character decoding %s failed."
::application::database::mc::DatabaseAlreadyOpen			"Il database '%s' è già aperto."
::application::database::mc::Properties						"Proprietà"
::application::database::mc::Preload							"Preload"
::application::database::mc::MissingEncoding					"Codifica %s mancante (uso %s)"
::application::database::mc::DescriptionTooLarge			"La descrizione è troppo lunga."
::application::database::mc::DescrTooLargeDetail			"Il campo contiene %d caratteri, ma solo %d sono permessi."
::application::database::mc::ClipbaseDescription			"Database temporaneo, non è salvato sul disco"
::application::database::mc::HardLinkDetected				"Impossibile caricare il file '%file1' perché è già caricato come file '%file2'. Questo può succedere solo se ci sono collegamenti tra i due." ;# hard links?
::application::database::mc::HardLinkDetectedDetail		"Se il database viene caricato due volte il programma potrebbe crashare per l'eccessivo utilizzo dei threads." ;# thread? lasciare così?
::application::database::mc::OverwriteExistingFiles		"Sovrascrivi i file esistenti nella cartella '%s'?"
::application::database::mc::SelectDatabases					"Selezione il database da aprire"
::application::database::mc::ExtractArchive					"Estrai l'archivio %s"
::application::database::mc::SelectVariant					"Selezione Variante"
::application::database::mc::Example							"Esempio"
::application::database::mc::UnsavedFiles						"Quest file PGN non è stato salvato"
::application::database::mc::FileIsRemoved					"Il file '%s' è rimosso. Usa la dialog di esportazione se vuoi salvare questo database."
::application::database::mc::FileIsNotWritable				"Il file '%s' non è scrivibile. Usa la dialog di esportazione se vuoi salvare questo database, o cambia i permessi per questo file."
::application::database::mc::OverwriteOriginalFile			"Important note: The original file '%s' will be overwritten." ;# NEW
::application::database::mc::SetupPgnOptions					"Probably the PGN options should be set before saving." ;# NEW
::application::database::mc::CloseAllDeletedGames			"Close all deleted games of database '%s'?" ;# NEW
::application::database::mc::CannotCompactDatabase			"Cannot compact database because the following games belonging to this database are modified:" ;# NEW

::application::database::mc::RecodingDatabase				"Registro %base da %from a %to"
::application::database::mc::RecodedGames						"%s partite registrate"

::application::database::mc::ChangeIcon						"Cambia Icona"
::application::database::mc::Recode								"Ricodifica"
::application::database::mc::EditDescription					"Modifica Descrizione"
::application::database::mc::EmptyClipbase					"Svuota Clipbase"

::application::database::mc::Maintenance						"Manutenzione"
::application::database::mc::StripMoveInfo					"Rimuovi informazioni mossa dal database '%s'"
::application::database::mc::StripPGNTags						"Rimuovi tag PGN dal database '%s'"
::application::database::mc::GamesStripped(0)				"Nessuna partita ripulita."
::application::database::mc::GamesStripped(1)				"Una partita ripulita."
::application::database::mc::GamesStripped(N)				"%s partite ripulite."
::application::database::mc::GamesRemoved(0)					"Nessuna partita rimossa."
::application::database::mc::GamesRemoved(1)					"Una partita rimossa."
::application::database::mc::GamesRemoved(N)					"%s partite rimosse."
::application::database::mc::AllGamesMustBeClosed			"Tutte le partite devono essere chiuse prima che l'operazione possa essere portata a termine."
::application::database::mc::ReallyCompact					"Vuoi davvero comprimere il database '%s'?"
::application::database::mc::ReallyCompactDetail(1)		"Solo una partita sarà eliminata."
::application::database::mc::ReallyCompactDetail(N)		"%s partite saranno eliminate."
::application::database::mc::RemoveSpace						"Alcuni spazi bianchi saranno rimossi."
::application::database::mc::CompactionRecommended			"Si raccomanda di comprimere il database."
::application::database::mc::SearchPGNTags					"Ricerca tag PNG"
::application::database::mc::SelectSuperfluousTags			"Seleziona tag superflui:"
::application::database::mc::WillBePermanentlyDeleted		"Nota: questa azione eliminerà definitavamente le informazioni dal database."
::application::database::mc::ReadWriteFailed					"Tentativo di settare il database come scrivibile fallito:"
::application::database::mc::NoExtraTagsFound				"No tags found for deletion." ;# NEW

::application::database::mc::T_Unspecific						"Non specificato"
::application::database::mc::T_Temporary						"Temporaneo"
::application::database::mc::T_Work								"Lavoro"
::application::database::mc::T_Clipbase						"Clipbase"
::application::database::mc::T_MyGames							"Mie partite"
::application::database::mc::T_Informant						"Informatore"
::application::database::mc::T_LargeDatabase					"Grande Database"
::application::database::mc::T_CorrespondenceChess			"Scacchi per corrispondenza"  
::application::database::mc::T_EmailChess						"Scacchi per email"
::application::database::mc::T_InternetChess					"Scacchi su internet"
::application::database::mc::T_ComputerChess					"Partite tra computer"
::application::database::mc::T_Chess960						"Scacchi 960"
::application::database::mc::T_PlayerCollection				"Monografia per giocatore"
# Female version of "Player Collection"
# Be sure that the translation starts with same term as the translation above.
::application::database::mc::T_PlayerCollectionFemale		"Monografia per giocatrice"
::application::database::mc::T_Tournament						"Torneo"
::application::database::mc::T_TournamentSwiss				"Torneo Svizzero"
::application::database::mc::T_GMGames							"Partite di GM"
::application::database::mc::T_IMGames							"Partite di IM"
::application::database::mc::T_BlitzGames						"Partite lampi"
::application::database::mc::T_Tactics							"Tattica"
::application::database::mc::T_Endgames						"Finali"
::application::database::mc::T_Analysis						"Analisi"
::application::database::mc::T_Training						"Allenamento"
::application::database::mc::T_Match							"Match"
::application::database::mc::T_Studies							"Studi"
::application::database::mc::T_Jewels							"Gemme"
::application::database::mc::T_Problems						"Problemi"
::application::database::mc::T_Patzer							"Brocchi"
::application::database::mc::T_Gambit							"Gambetti"
::application::database::mc::T_Important						"Importanti"
::application::database::mc::T_Openings						"Aperture"
::application::database::mc::T_OpeningsWhite					"Aperture bianco"
::application::database::mc::T_OpeningsBlack					"Aperture nero"
::application::database::mc::T_Bughouse						"Mangia e passa"
::application::database::mc::T_Antichess						"Vinciperdi"
::application::database::mc::T_PGNFile							"File PGN"
::application::database::mc::T_ThreeCheck						"Tre scacchi"
::application::database::mc::T_Crazyhouse						"Crazyhouse"

::application::database::mc::OpenDatabase						"Apri Database"
::application::database::mc::OpenRecentDatabase				"Apri Database Recente"
::application::database::mc::NewDatabase						"Nuovo Database"
::application::database::mc::CloseDatabase					"Chiudi Database '%s'"
::application::database::mc::SetReadonly						"Imposta Database '%s' in sola lettura"
::application::database::mc::SetWriteable						"Imposta Database '%s' in scrittura"

::application::database::mc::OpenReadonly						"Apri sola lettura"
::application::database::mc::OpenWriteable					"Apri in scrittura"

::application::database::mc::UpgradeDatabase					"%s è un database in un vecchio formato che non può essere aperto per la scrittura.\n\nL'aggiornamento creerà una nuova versione del database e rimuoverà i file originali.\n\nQuesta operazione può richiedere del tempo, ma è necessario eseguirla una volta sola.\n\nVuoi aggiornare il database ora?"
::application::database::mc::UpgradeDatabaseDetail			"\"No\" aprirà il database per la sola lettura, non puoi settarlo come scrivibile."

::application::database::mc::MoveInfo(evaluation)			"Valutazione"
::application::database::mc::MoveInfo(playersClock)		"Orologio del giocatore"
::application::database::mc::MoveInfo(elapsedGameTime)	"Tempo di gioco utilizzato"
::application::database::mc::MoveInfo(elapsedMoveTime)	"Tempo per mossa utilizzato"
::application::database::mc::MoveInfo(elapsedMilliSecs)	"Tempo utilizzato in millisecondi"
::application::database::mc::MoveInfo(clockTime)			"Tempo dell'orologio"
::application::database::mc::MoveInfo(corrChessSent)		"Mossa per corrispondenzza inviata"
::application::database::mc::MoveInfo(videoTime)			"Tempo Video"

### application::database::games #######################################
::application::database::games::mc::Control						"Controllo"
::application::database::games::mc::GameNumber					"Numero partita"

::application::database::games::mc::GotoFirstPage				"Vai alla prima pagina di partite"
::application::database::games::mc::GotoLastPage				"Vai all'ultima pagina di partite"
::application::database::games::mc::PreviousPage				"Pagina di partite precedente"
::application::database::games::mc::NextPage						"Prossima pagina di partite"
::application::database::games::mc::GotoCurrentSelection		"Vai alla selezione attuale"
::application::database::games::mc::UseVerticalScrollbar		"Usa scrollbar verticale"
::application::database::games::mc::UseHorizontalScrollbar	"Usa scrollbar orizzontale"
::application::database::games::mc::GotoEnteredGameNumber	"Vai alla partita numero..."

### application::database::players #####################################
::application::database::player::mc::EditPlayer			"Modifica Giocatore"
::application::database::player::mc::Score				"Punteggio"

### application::database::annotators ##################################
::application::database::annotator::mc::F_Annotator	"Commentatore"
::application::database::annotator::mc::F_Frequency	"Frequenza"

::application::database::annotator::mc::Find				"Cerca"
::application::database::annotator::mc::FindAnnotator	"Cerca Commentatore"
::application::database::annotator::mc::NoAnnotator	"No annotator" ;# NEW

### application::database::positions ###################################
::application::database::position::mc::NoCastle			"No castle" ;# NEW

::application::database::position::mc::F_Position		"Position" ;# NEW
::application::database::position::mc::F_Description	"Description" ;# NEW
::application::database::position::mc::F_BackRank		"Back Rank" ;# NEW
::application::database::position::mc::F_Frequency		"Frequency" ;# NEW

### application::pgn ###################################################
::application::pgn::mc::Command(move:comment)			"Imposta commento"
::application::pgn::mc::Command(move:marks)				"Imposta codici"
::application::pgn::mc::Command(move:annotation)		"Imposta annotazioni/commenti/codici"
::application::pgn::mc::Command(move:append)				"Aggiungi mossa"
::application::pgn::mc::Command(move:append:n)			"Aggiungi Mosse"
::application::pgn::mc::Command(move:exchange)			"Cambia mossa"
::application::pgn::mc::Command(variation:new)			"Aggiungi variante"
::application::pgn::mc::Command(variation:new:n)		"Add Variations" ;# NEW
::application::pgn::mc::Command(variation:replace)		"Sostituisci mosse"
::application::pgn::mc::Command(variation:truncate)	"Interrompi variante"
::application::pgn::mc::Command(variation:first)		"Rendi prima variante"
::application::pgn::mc::Command(variation:promote)		"Promuovi variante a linea principale"
::application::pgn::mc::Command(variation:remove)		"Elimina variante"
::application::pgn::mc::Command(variation:remove:n)	"Elimina Varianti"
::application::pgn::mc::Command(variation:merge)		"Merge variation(s)" ;# NEW
::application::pgn::mc::Command(variation:mainline)	"Nuova linea principale"
::application::pgn::mc::Command(variation:insert)		"Inserisci mosse"
::application::pgn::mc::Command(variation:exchange)	"Cambia mosse"
::application::pgn::mc::Command(strip:moves)				"Mosse dall'inizio"
::application::pgn::mc::Command(strip:truncate)			"Mosse alla fine"
::application::pgn::mc::Command(strip:annotations)		"Annotazioni"
::application::pgn::mc::Command(strip:info)				"Informazioni Mossa"
::application::pgn::mc::Command(strip:marks)				"Codici"
::application::pgn::mc::Command(strip:comments)			"Commenti"
::application::pgn::mc::Command(strip:language)       "Language" ;# NEW
::application::pgn::mc::Command(strip:variations)		"Varianti"
::application::pgn::mc::Command(copy:comments)			"Copia Commenti"
::application::pgn::mc::Command(move:comments)			"Muovi Commenti"
::application::pgn::mc::Command(game:clear)				"Pulisci partita"
::application::pgn::mc::Command(game:merge)				"Unisci Partite"
::application::pgn::mc::Command(game:transpose)			"Trasponi Partita"

::application::pgn::mc::StartTrialMode						"Inizia modalità di prova"
::application::pgn::mc::StopTrialMode						"Interrompi modalità di prova"
::application::pgn::mc::Strip									"Rimuovi"
::application::pgn::mc::InsertDiagram						"Inserisci diagramma"
::application::pgn::mc::InsertDiagramFromBlack			"Inserisci diagramma dalla prospettiva del nero"
::application::pgn::mc::SuffixCommentaries				"Commenti dopo mossa"
::application::pgn::mc::StripOriginalComments			"Rimuovi commenti originali"

::application::pgn::mc::LanguageSelection					"Selezione Lingue"
::application::pgn::mc::MoveInfoSelection					"Selezione Info Mossa"
::application::pgn::mc::MoveNotation						"Muovi Notazione"
::application::pgn::mc::CollapseVariations				"Comprimi Varianti"
::application::pgn::mc::ExpandVariations					"Espandi Varianti"
::application::pgn::mc::EmptyGame							"Partita Vuota"

::application::pgn::mc::NumberOfMoves						"Numero di semimosse (nella linea principale):"
::application::pgn::mc::InvalidInput						"Input non valido '%d'."
::application::pgn::mc::MustBeEven							"Input deve essere un numero pari."
::application::pgn::mc::MustBeOdd							"Input deve essere un numero dispari."
::application::pgn::mc::CannotOpenCursorFiles			"Impossibile aprire il file del cursore: %s"
::application::pgn::mc::ReallyReplaceMoves				"Vuoi davvero sostituire le mosse della partita corrente?"
::application::pgn::mc::CurrentGameIsNotModified		"La partita corrente non è stata modificata."
::application::pgn::mc::ShufflePosition					"Mischia posizione..."

::application::pgn::mc::EditAnnotation						"Modifica annotazioni"
::application::pgn::mc::EditMoveInformation				"Modifica informazioni mossa"
::application::pgn::mc::EditCommentBefore					"Modifica commento prima della mossa"
::application::pgn::mc::EditCommentAfter					"Modifica commento dopo la mossa"
::application::pgn::mc::EditPrecedingComment				"Modifica commento precedente"
::application::pgn::mc::EditTrailingComment				"Modifica commento successivo"
::application::pgn::mc::EditMarks							"Modifica codici"
::application::pgn::mc::Display								"Display"
::application::pgn::mc::None									"nessuno"

::application::pgn::mc::MoveInfo(eval)						"Valutazione"
::application::pgn::mc::MoveInfo(clk)						"Orologio Giocatori"
::application::pgn::mc::MoveInfo(emt)						"Tempo Passato"
::application::pgn::mc::MoveInfo(ccsnt)					"Mossa Mandata per Corrispondenza"
::application::pgn::mc::MoveInfo(video)					"Tempo Video"

### application::tree ##################################################
::application::tree::mc::Total								"Totale"
::application::tree::mc::Control								"Controllo"
::application::tree::mc::ChooseReferenceBase				"Scegli database di riferimento"
::application::tree::mc::ReferenceBaseSwitcher			"Selezione database di riferimento"
::application::tree::mc::Numeric								"Numerico"
::application::tree::mc::Bar									"Barra"
::application::tree::mc::StartSearch						"Comincia ricerca"
::application::tree::mc::StopSearch							"Interrompi ricerca"
::application::tree::mc::UseExactMode						"Usa ricerca di posizione"
::application::tree::mc::UseFastMode						"Usa ricerca accelerata"
::application::tree::mc::UseQuickMode						"Usa ricerca rapida"
::application::tree::mc::AutomaticSearch					"Ricerca automatica"
::application::tree::mc::LockReferenceBase				"Blocca il database di riferimento"
::application::tree::mc::SwitchReferenceBase				"Cambia database di riferimento"
::application::tree::mc::TransparentBar					"Barra trasparente"
::application::tree::mc::MonochromeStyle					"Use monochrome style" ;# NEW
::application::tree::mc::NoGamesFound						"Nessuna partita trovata"
::application::tree::mc::NoGamesAvailable					"Nessuna partita disponibile"
::application::tree::mc::Searching							"Ricerca"
::application::tree::mc::VariantsNotYetSupported		"Variante di scacchi non ancora supportata."
::application::tree::mc::End									"fine"
::application::tree::mc::ShowAllMoveOrders				"Show all move orders" ;# NEW
::application::tree::mc::MoveOrders							"Move Orders" ;# NEW
::application::tree::mc::ComputeSpread						"Compute Spread" ;# NEW
::application::tree::mc::ShowMoveTree						"Show move tree" ;# NEW
::application::tree::mc::ShowMoveOrders					"Show move orders" ;# NEW
::application::tree::mc::SearchInsideVariations			"Search inside variations" ;# NEW

::application::tree::mc::FromWhitesPerspective			"Dalla prospettiva del bianco"
::application::tree::mc::FromBlacksPerspective			"Dalla prospettiva del nero"
::application::tree::mc::FromSideToMovePerspective		"Dalla prospettiva del lato col tratto"
::application::tree::mc::FromWhitesPerspectiveTip		"Punteggio dalla prospettiva del bianco"
::application::tree::mc::FromBlacksPerspectiveTip		"Punteggio dalla prospettiva del nero"
::application::tree::mc::EmphasizeMoveOfGame				"Enfatizza mossa della partita"

::application::tree::mc::TooltipAverageRating			"Media ELO (%s)"
::application::tree::mc::TooltipBestRating				"Miglior ELO (%s)"

::application::tree::mc::F_Number							"#"
::application::tree::mc::F_Move								"Mossa"
::application::tree::mc::F_Eco								"ECO"
::application::tree::mc::F_Frequency						"Frequenza"
::application::tree::mc::F_Ratio								"Percentuale"
::application::tree::mc::F_Score								"Risultato"
::application::tree::mc::F_Draws								"Patte"
::application::tree::mc::F_Result							"Risultato"
::application::tree::mc::F_Performance						"Performance"
::application::tree::mc::F_AverageYear						"\u00f8 Anno"
::application::tree::mc::F_LastYear							"Giocata per ultimo"
::application::tree::mc::F_BestPlayer						"Miglior giocatore"
::application::tree::mc::F_FrequentPlayer					"Giocata frequentemente da"

::application::tree::mc::T_Number							"Numerazione"
::application::tree::mc::T_AverageYear						"Media anni"
::application::tree::mc::T_FrequentPlayer					"Giocata frequentemente da"

### database::switcher #################################################
::database::switcher::mc::Empty								"vuoto"
::database::switcher::mc::None								"nessuno"
::database::switcher::mc::Failed								"fallito"

::database::switcher::mc::UriRejectedDetail(open)		"Solo database Scidb possono essere aperti:"
::database::switcher::mc::UriRejectedDetail(import)	"Solo database Scidb, ma non database ChessBase, possono essere importati"
::database::switcher::mc::EmptyUriList						"Gli archivi trascinati sono vuoti."
::database::switcher::mc::CopyGames							"Copia partite"
::database::switcher::mc::CopyGamesFromTo					"Copia partite da '%src' a '%dst'"
::database::switcher::mc::CopiedGames						"%s game(s) copied"
::database::switcher::mc::NoGamesCopied					"Nessuna partita copiata"
::database::switcher::mc::CopyGamesFrom					"Copia partite da '%s'"
::database::switcher::mc::ImportGames						"Importa partite"
::database::switcher::mc::ImportFiles						"Importa i file:"

::database::switcher::mc::ImportOneGameTo(0)				"Copiare una partita in '%dst'?"
::database::switcher::mc::ImportOneGameTo(1)				"Copiare circa una partita in '%dst'?"
::database::switcher::mc::ImportGamesTo(0)				"Copiare %num partite in '%dst'?"
::database::switcher::mc::ImportGamesTo(1)				"Copiare circa %num partite in '%dst'?"

::database::switcher::mc::NumGames(0)						"nessuno"
::database::switcher::mc::NumGames(1)						"una partita"
::database::switcher::mc::NumGames(N)						"%s partite"

::database::switcher::mc::SelectGames(all)				"Tutte le partite"
::database::switcher::mc::SelectGames(filter)			"Solo partite filtrate"
::database::switcher::mc::SelectGames(all,variant)		"Solo variante %s"
::database::switcher::mc::SelectGames(filter,variant)	"Solo partite filtrate della variante %s"
::database::switcher::mc::SelectGames(complete)			"Database completato"

::database::switcher::mc::GameCount							"Partite"
::database::switcher::mc::DatabasePath						"Indirizzo Database"
::database::switcher::mc::DeletedGames						"Partite Eliminate"
::database::switcher::mc::ChangedGames						"Partite Cambiate"
::database::switcher::mc::AddedGames						"Partite Aggiunte"
::database::switcher::mc::Description						"Descrizione"
::database::switcher::mc::Created							"Creata"
::database::switcher::mc::LastModified						"Ultima Modifica"
::database::switcher::mc::Encoding							"Codifica"
::database::switcher::mc::YearRange							"Range Anni"
::database::switcher::mc::RatingRange						"Range Punteggio"
::database::switcher::mc::Result								"Risultato"
::database::switcher::mc::Score								"Score"
::database::switcher::mc::Type								"Tipo"
::database::switcher::mc::ReadOnly							"Sola Lettura"

### board ##############################################################
::board::mc::CannotReadFile			"Impossibile leggere file '%s':"
::board::mc::CannotFindFile			"Impossibile trovare file '%s'"
::board::mc::FileWillBeIgnored		"'%s' sarà ignorato (ID doppio)"
::board::mc::IsCorrupt					"'%s' è corrotto (stile %s sconosciuto '%s')"
::board::mc::SquareStyleIsUndefined	"Stile casella '%s' non esiste più"
::board::mc::PieceStyleIsUndefined	"Stile pezzo '%s' non esiste più"
::board::mc::ThemeIsUndefined			"Tema scacchiera '%s' non esiste più"

::board::mc::ThemeManagement			"Gestore Temi"
::board::mc::Setup						"Setup"

::board::mc::WorkingSet					"Selezione di lavoro"

### board::options #####################################################
::board::options::mc::Coordinates			"Coordinate"
::board::options::mc::SolidColor				"Tinta Unita"
::board::options::mc::EditList				"Modifica lista"
::board::options::mc::Embossed				"In Rilievo"
::board::options::mc::Small					"Small Letters" ;# NEW
::board::options::mc::Highlighting			"Evidenzia"
::board::options::mc::Border					"Bordo"
::board::options::mc::SaveWorkingSet		"Salva set di lavoro"
::board::options::mc::SelectedSquare		"Seleziona casa"
::board::options::mc::ShowBorder				"Mostra bordo"
::board::options::mc::ShowCoordinates		"Mostra coordinate"
::board::options::mc::UseSmallLetters		"Use Small Letters" ;# NEW
::board::options::mc::ShowMaterialValues	"Mostra valore dei pezzi"
::board::options::mc::ShowMaterialBar		"Mostra barra del materiale"
::board::options::mc::ShowSideToMove		"Mostra lato con il tratto"
::board::options::mc::ShowSuggestedMove	"Mostra mossa suggerita"
::board::options::mc::ShowPieceShadow		"Show Piece Shadow" ;# NEW
::board::options::mc::ShowPieceContour		"Show Piece Contour" ;# NEW
::board::options::mc::SuggestedMove			"Mossa suggerita"
::board::options::mc::Basic					"Basilare"
::board::options::mc::PieceStyle				"Stile dei pezzi"
::board::options::mc::SquareStyle			"Stile delle case"
::board::options::mc::Styles					"Stili"
::board::options::mc::Show						"Anteprima"
::board::options::mc::ChangeWorkingSet		"Modifica set di lavoro"
::board::options::mc::CopyToWorkingSet		"Copia su set di lavoro"
::board::options::mc::NameOfPieceStyle		"Inserisci nome dello stile pezzi"
::board::options::mc::NameOfSquareStyle	"Inserisci nome dello stile case"
::board::options::mc::NameOfThemeStyle		"Inserisci nome del tema"
::board::options::mc::PieceStyleSaved		"Stile pezzi '%s' salvano sotto '%s'"
::board::options::mc::SquareStyleSaved		"Stile case '%s' salvato sotto '%s'"
::board::options::mc::ChooseColors			"Scegli colori"
::board::options::mc::SupersedeSuggestion	"Sostituisci/usa colori suggeridi dallo stile della casa"
::board::options::mc::CannotDelete			"Non posso eliminare '%s'."
::board::options::mc::IsWriteProtected		"Il file '%s' è protetto per la scrittura"
::board::options::mc::ConfirmDelete			"Sei sicuro di voler eliminare '%s'?"
::board::options::mc::NoPermission			"Non posso eliminare '%s'.\nPermesso negato."
::board::options::mc::BoardSetup				"Configurazione scacchiera" ;# NEW changed to "Board Options / Select Theme"
::board::options::mc::OpenTextureDialog	"Apri finestra Texture"

::board::options::mc::YouCannotReverse		"Non puoi ritornare su questa azione. Il file '%s' sarà rimosso fisicamente."

::board::options::mc::CannotUsePieceWorkingSet "Non posso creare un nuovo tema con %s selezionato per lo stile dei pezzi."
::board::options::mc::CannotUsePieceWorkingSet "Prima devi salvare il nuovo stile pezzi, o scegliere un altro stile pezzi."

::board::options::mc::CannotUseSquareWorkingSet "Non posso creare un nuovo tema con %s selezionato per lo stile delle case."
::board::options::mc::CannotUseSquareWorkingSet "Prima devi salvare il nuovo stile case, o scegliere un altro stile case."

### board::piece #######################################################
::board::piece::mc::Start						"Inizia"
::board::piece::mc::Stop						"Ferma"
::board::piece::mc::HorzOffset				"Offset Orizzontale"
::board::piece::mc::VertOffset				"Offset Verticale"
::board::piece::mc::Gradient					"Gradiente"
::board::piece::mc::Fill						"Riempimento"
::board::piece::mc::Stroke						"Tratto"
::board::piece::mc::Contour					"Contorno"
::board::piece::mc::WhiteShape				"Forma bianca"
::board::piece::mc::PieceSelection			"Selezione pezzi"
::board::piece::mc::BackgroundSelection	"Selezione sfondo"
::board::piece::mc::Zoom						"Zoom"
::board::piece::mc::Shadow						"Ombre"
::board::piece::mc::Opacity					"Opacità"
::board::piece::mc::ShadowDiffusion			"Diffusione Ombra"
::board::piece::mc::PieceStyleConf			"Configurazione stile pezzi"
::board::piece::mc::Offset						"Offset"
::board::piece::mc::Rotate						"Ruota"
::board::piece::mc::CloseDialog				"Chiudi finestra di dialogo e perdere le modifiche?"
::board::piece::mc::OpenTextureDialog		"Apri finestra Texture"

### board::square ######################################################
::board::square::mc::SolidColor			"Tinta unita"
::board::square::mc::CannotReadFile		"Impossibile leggere il file"
::board::square::mc::Zoom					"Zoom"
::board::square::mc::Offset				"Offset"
::board::square::mc::Rotate				"Ruota"
::board::square::mc::Borderline			"Bordo"
::board::square::mc::Width					"Larghezza"
::board::square::mc::Opacity				"Opacità"
::board::square::mc::GapBetweenSquares	"Spazio tra le case" ;# NEW text: "Show always gap between squares"
::board::square::mc::GapColor				"Gap color" ;# NEW
::board::square::mc::Highlighting		"Evidenzia"
::board::square::mc::Selected				"Selezionato"
::board::square::mc::SuggestedMove		"Mossa suggerita"
::board::square::mc::Show					"Anteprima"
::board::square::mc::SquareStyleConf	"Configurazione stile casa"
::board::square::mc::CloseDialog			"Chiudere la finestra e perdere i cambiamenti?"

### board::texture #####################################################
::board::texture::mc::PreselectedOnly "Solo preselezionato"

### pgn-setup ##########################################################
::pgn::setup::mc::Configure(editor)				"Personalizza l'editor"
::pgn::setup::mc::Configure(browser)			"Personalizza il testo in mostra"
::pgn::setup::mc::TakeOver(editor)				"Imposta configurazione dallo Sfoglia Partite"
::pgn::setup::mc::TakeOver(browser)				"Imposta configurazione dall'Editor Partite"
::pgn::setup::mc::Pixel								"pixel"
::pgn::setup::mc::Spaces							"spazi"
::pgn::setup::mc::RevertSettings					"Torna alla configurazione iniziale"
::pgn::setup::mc::ResetSettings					"Torna alla configurazione di produzione"
::pgn::setup::mc::DiscardAllChanges				"Annulla tutte le modifiche applicate?"
::pgn::setup::mc::ThreefoldRepetition			"Triplice ripetizione"
::pgn::setup::mc::FivefoldRepetition			"Fivefold repetition" ;# NEW
::pgn::setup::mc::FiftyMoveRule					"Regola delle 50 mosse"

::pgn::setup::mc::Setup(Appearance)				"Aspetto"
::pgn::setup::mc::Setup(Layout)					"Layout"
::pgn::setup::mc::Setup(Diagrams)				"Diagrammi"
::pgn::setup::mc::Setup(MoveStyle)				"Stile Mossa"

::pgn::setup::mc::Setup(Fonts)					"Fonts"
::pgn::setup::mc::Setup(font-and-size)			"Font e dimensione testo"
::pgn::setup::mc::Setup(figurine-font)			"Figurine (normale)"
::pgn::setup::mc::Setup(figurine-bold)			"Figurine (grassetto)"
::pgn::setup::mc::Setup(symbol-font)			"Simboli"

::pgn::setup::mc::Setup(Colors)					"Colori"
::pgn::setup::mc::Setup(Highlighting)			"Evidenziazione"
::pgn::setup::mc::Setup(start-position)		"Posizione Iniziale"
::pgn::setup::mc::Setup(variations)				"Varianti"
::pgn::setup::mc::Setup(numbering)				"Numerazione"
::pgn::setup::mc::Setup(brackets)				"Parentesi"
::pgn::setup::mc::Setup(illegal-move)			"Mosse Illegali"
::pgn::setup::mc::Setup(comments)				"Commenti"
::pgn::setup::mc::Setup(annotation)				"Annotazioni"
::pgn::setup::mc::Setup(nagtext)					"NAG-Testo"
::pgn::setup::mc::Setup(marks)					"Codici"
::pgn::setup::mc::Setup(move-info)				"Informazioni Mossa"
::pgn::setup::mc::Setup(result)					"Risultato"
::pgn::setup::mc::Setup(current-move)			"Mossa Corrente"
::pgn::setup::mc::Setup(next-moves)				"Prossima Mossa"
::pgn::setup::mc::Setup(empty-game)				"Partita Vuota"

::pgn::setup::mc::Setup(Hovers)					"Al passaggio del mouse"
::pgn::setup::mc::Setup(hover-move)				"Mossa"
::pgn::setup::mc::Setup(hover-comment)			"Commento"
::pgn::setup::mc::Setup(hover-move-info)		"Informazioni Mossa"

::pgn::setup::mc::Section(ParLayout)			"Layout Paragrafo"
::pgn::setup::mc::ParLayout(use-spacing)		"Usa spaziatura di paragrafo"
::pgn::setup::mc::ParLayout(column-style)		"Stile colonna"
::pgn::setup::mc::ParLayout(tabstop-1)			"Indentazione per Mossa Bianca"
::pgn::setup::mc::ParLayout(tabstop-2)			"Indentazione per Mossa Nera"
::pgn::setup::mc::ParLayout(mainline-bold)	"Grassetto per mosse nella linea principale"

::pgn::setup::mc::Section(Variations)			"Layout Varianti"
::pgn::setup::mc::Variations(width)				"Larghezza Indentazione"
::pgn::setup::mc::Variations(level)				"Livello Indentazione"

::pgn::setup::mc::Section(Display)				"Visualizzazione"
::pgn::setup::mc::Display(numbering)			"Mostra Numerazione Varianti"
::pgn::setup::mc::Display(markers)				"Show Square Markers" ;# NEW
::pgn::setup::mc::Display(moveinfo)				"Mostra Informazioni Mossa"
::pgn::setup::mc::Display(nagtext)				"Mostra testo per commenti NAG inusitati"

::pgn::setup::mc::Section(Diagrams)				"Diagrammi"
::pgn::setup::mc::Diagrams(show)					"Mostra diagrammi"
# Note for translators: "Emoticons" can be simply translated to "Smileys"
::pgn::setup::mc::Emoticons(show)				"Individua Emoticons"
::pgn::setup::mc::Diagrams(square-size)		"Grandezza Casa"
::pgn::setup::mc::Diagrams(indentation)		"Larghezza Indentazione"


### engine #############################################################
::engine::mc::Information				"Informazioni"
::engine::mc::Features					"Funzioni"
::engine::mc::Options					"Opzioni"

::engine::mc::Name						"Nome"
::engine::mc::Identifier				"Identificativo"
::engine::mc::Author						"Autore"
::engine::mc::Webpage					"Pagina web"
::engine::mc::Email						"Email"
::engine::mc::Country					"Paese"
::engine::mc::Rating						"Punteggio"
::engine::mc::Logo						"Logo"
::engine::mc::Protocol					"Protocollo"
::engine::mc::Parameters				"Parametri"
::engine::mc::Command					"Comando"
::engine::mc::Directory					"Directory"
::engine::mc::Variants					"Varianti"
::engine::mc::LastUsed					"Ultimo utilizzo"

::engine::mc::Variant(standard)		"Standard"
::engine::mc::Variant(chess960)		"Scacchi 960"
::engine::mc::Variant(bughouse)		"Mangia e passa"
::engine::mc::Variant(crazyhouse)	"Crazyhouse"
# NOTE: Suicide is Antichess according to FICS rules
# NOTE: "Giveaway" is Antichess according to internatianal rules.
# NOTE: "Losers" is Antichess according to ICC rules
# NOTE: You may tarnslate "Suicide", "Giveaway", anmd "Losers" with the same term.
::engine::mc::Variant(suicide)		"Vinciperdi"
::engine::mc::Variant(giveaway)		"Vinciperdi"
::engine::mc::Variant(losers)			"Vinciperdi"
::engine::mc::Variant(3check)			"Tre scacchi"

::engine::mc::Edit						"Modifica"
::engine::mc::View						"Mostra"
::engine::mc::New							"Nuovo"
::engine::mc::Rename						"Rinomina"
::engine::mc::Delete						"Elimina"
::engine::mc::Select(engine)			"Seleziona motore"
::engine::mc::Select(profile)			"Seleziona profilo"
::engine::mc::ProfileName				"Nome profilo"
::engine::mc::NewProfileName			"Nuovo nome profilo"
::engine::mc::OldProfileName			"Vecchio nome profilo"
::engine::mc::CopyFrom					"Copia da"
::engine::mc::NewProfile				"Nuovo profilo"
::engine::mc::RenameProfile			"Rinomina profilo"
::engine::mc::EditProfile				"Modifica profilo '%s'"
::engine::mc::ProfileAlreadyExists	"Un profilo col nome '%s' esiste già."
::engine::mc::ChooseDifferentName	"Per favore scegli un nome diverso."
::engine::mc::ReservedName				"Il nome '%s' è riservato e non può essere usato."
::engine::mc::ReallyDeleteProfile	"Vuoi davvero eliminare il profilo '%s'?"
::engine::mc::SortName					"Ordina per nome"
::engine::mc::SortElo					"Ordina per punteggio Elo"
::engine::mc::SortRating				"Ordina per punteggio CCRL"
::engine::mc::OpenUrl					"Apri URL (browser web)"

::engine::mc::AdminEngines				"Configura &Motori"
::engine::mc::SetupEngine				"Imposta motore %s"
::engine::mc::ImageFiles				"File di immagine"
::engine::mc::SelectEngine				"Seleziona Motore"
::engine::mc::SelectEngineLogo		"Seleziona Logo Motore"
::engine::mc::EngineDictionary		"Dizionario Motore"
::engine::mc::EngineFilter				"Filtro Motori"
::engine::mc::EngineLog					"Console del motore"
::engine::mc::Probing					"Sondaggio" ;# Probing?
::engine::mc::NeverUsed					"Mai usato"
::engine::mc::OpenFsbox					"Apri interfaccia di selezione file"
::engine::mc::ResetToDefault			"Riporta a valori di default"
::engine::mc::ShowInfo					"Mostra \"Info\""
::engine::mc::TotalUsage				"%s volte in totale"
::engine::mc::Memory						"Memoria (MB)"
::engine::mc::CPUs						"CPUs"
::engine::mc::Priority					"Priorità CPU"
::engine::mc::ClearHash					"Pulisci tabelle hash"

::engine::mc::ConfirmNewEngine		"Conferma nuovo motore"
::engine::mc::EngineAlreadyExists	"Un inserimento con questo motore esiste già."
::engine::mc::CopyFromEngine			"Fai una copia di questo inserimento"
::engine::mc::CannotOpenProcess		"Impossibile iniziare il processo."
::engine::mc::DoesNotRespond			"Questo motore non risponde al protocollo UCI né al protocollo XBoard/WinBoard."
::engine::mc::DiscardChanges			"The current item has changed.\n\nReally discard changes?"
::engine::mc::ReallyDelete				"Vuoi davvero eliminare il motore '%s'?"
::engine::mc::EntryAlreadyExists		"Un inserimento con il nome '%s' esiste già."
::engine::mc::NoFeaturesAvailable	"Questo motore non fornisce alcuna funzione, neanche la modalità di analisi è disponibile. Impossibile usare questo motore per analizzare posizioni."
::engine::mc::NoStandardChess			"Questo motore non supporta gli scacchi standard."
::engine::mc::NoEngineAvailable		"Nessun motore disponibile."
::engine::mc::FailedToCreateDir		"Impossibile creare directory '%s'."
::engine::mc::ScriptErrors				"Qualsiasi errore durante il salvataggio sarà mostrato qui."
::engine::mc::CommandNotAllowed		"L'uso del comando '%s' non è permesso."
::engine::mc::ResetToDefaultContent	"Reimposta contenuto di default"
::engine::mc::PleaseBePatient			"Please be patient, 'Wine' needs some time." ;# NEW
::engine::mc::TryAgain					"The first start of 'Wine' needs some time, maybe it works if you try it again." ;# NEW
::engine::mc::CannotUseWindowsExe	"Cannot use Windows executable without 'Wine'." ;# NEW
::engine::mc::InstallWine				"Please install 'Wine' beforehand." ;# NEW

::engine::mc::ProbeError(registration)			"Questo motore richiede una registrazione."
::engine::mc::ProbeError(copyprotection)		"Questo motore ha una protezione anti-copia."

::engine::mc::FeatureDetail(analyze)			"Questo motore dispone di una modalità di analisi."
::engine::mc::FeatureDetail(multiPV)			"Ti permette di vedere le valutazioni del motore e le principali varianti (PVs) a partire da quella ritenuta migliore. Questo motore può mostrare al massimo %s varianti principali."
::engine::mc::FeatureDetail(pause)				"Questo permette una corretta gestione della pausa/riavvio: il motore non pensa, o consuma altrimenti risorse e tempo della CPU. L'analisi corrente è sospesa e gli orologi sono fermati per entrambi i giocatori."
::engine::mc::FeatureDetail(playOther)			"Questo motore è in grado di giocare la tua mossa. Il tuo orologio sarà in azione mentre il motore pensa alla mossa da te inserita."
::engine::mc::FeatureDetail(hashSize)			"Questa funzione permette di informare su quanta memoria (al massimo) è possibile utilizzare per le hash tables. Questo motore permette un intervallo da %min a %max MB."
::engine::mc::FeatureDetail(clearHash)			"L'utente può pulire le hash tables mentre il motore è attivo."
::engine::mc::FeatureDetail(threads)			"Permette di configurare il numero di thread che il motore userà durante la sua analisi. Il motore sta usando da %min a %max threads."
::engine::mc::FeatureDetail(smp)					"Con questo motore più di una CPU (core) può essere usata."
::engine::mc::FeatureDetail(limitStrength)	"Il motore è in grado di limitare la propria forza di gioco a uno specifico equivalente ELO tra %min-%max."
::engine::mc::FeatureDetail(skillLevel)		"Il motore dà la possibilità di diminuire la propria forza di gioco, al punto in cui può essere battuto piuttosto facilmente."
::engine::mc::FeatureDetail(ponder)				"Pensare significa semplicemente usare il tempo dell'utente per considerare le sue mosse e quindi avere un vantaggio di pre-processing quando è il turno di muovere del motore. Si chiama anche Mente Permanente."
::engine::mc::FeatureDetail(chess960)			"Scacchi 960 (o Scacchi Fischer) è una variante degli scacchi. Si utilizza la stessa scacchiera e gli stessi pezzi, ma la posizione iniziale dei pezzi è randomica pur rimanendo sulle stesse linee con alcune restrizioni che preservano il diritto di arrocco, risultando in 960 posizioni uniche."
::engine::mc::FeatureDetail(bughouse)			"Mangia e Passa è una variante degli scacchi giocata su due scacchiere da quattro giocatori in squadre di due. Si applicano le regole degli scacchi ortodossi, eccetto che i pezzi catturati su una scacchiera passano all'altra, i giocatori di quest'altra scacchiera hanno l'opzione di reintrodurre il pezzo."
::engine::mc::FeatureDetail(crazyhouse)		"Crazyhouse è una variante degli scacchi simile a Mangia e Passa con solo due giocatori. Di fatto implementa una regola degli shogi (scacchi giapponesi), per cui un giocatore può reintrodurre nel gioco un pezzo catturato come proprio."
::engine::mc::FeatureDetail(suicide)			"Scacchi Suicidio (chiamato anche Vinciperdi, Prendimi, Scacchi alla rovescia...) ha una semplice regola: le mosse di cattura sono obbligatorie e l'obiettivo è di perdere tutti i pezzi. Non ci sono scacchi, il re è catturato come un pezzo normale. In caso di stallo vince il lato con meno pezzi (in accordo alle regole di FICS)."
::engine::mc::FeatureDetail(giveaway)			"Scacchi 'Giveaway' (una variante di Vinciperdi) è come Scacchi Suicidio, ma in caso di stallo il difensore vince (in accordo alle regole internazionali)."
::engine::mc::FeatureDetail(losers)				"Vinciperdi è una variante di Vinciperdi dove l'obiettivo è di vincere la partita, ma con diverse condizioni. L'obiettivo è di perdere tutti i tuoi pezzi (tranne il re) ma in Vinciperdi puoi anche vincere prendendo matto (in accordo alle regole di ICC)."
::engine::mc::FeatureDetail(3check)				"Caratteristiche di questa variante degli scacchi: un giocatore vince se dà scacco al suo avversario tre volte."
::engine::mc::FeatureDetail(playingStyle)		"Questo motore disponde di diversi stili di gioco, ad esempio %s. Leggi il manuale del motore per una spiegazione sui diversi stili."

### analysis ###########################################################
::application::analysis::mc::Control						"Controllo"
::application::analysis::mc::Information					"Informazioni"
::application::analysis::mc::SetupEngine					"Impostazioni" ;# NEW changed to "Setup engine"
::application::analysis::mc::Pause							"Pausa"
::application::analysis::mc::Resume							"Riattiva"
::application::analysis::mc::LockEngine					"Blocca motore sulla posizione attuale"
::application::analysis::mc::CloseEngine					"Power down motor" ;# NEW
::application::analysis::mc::MultipleVariations			"Varianti multiple (multi-pv)"
::application::analysis::mc::HashFullness					"Completezza Hash"
::application::analysis::mc::NodesPerSecond				"Nodes per second" ;# NEW
::application::analysis::mc::TablebaseHits				"Tablebase hits" ;# NEW
::application::analysis::mc::Hash							"Hash:"
::application::analysis::mc::Lines							"Varianti:"
::application::analysis::mc::MateIn							"%color matto in %n"
::application::analysis::mc::BestScore						"Miglior punteggio (delle varianti attuali)"
::application::analysis::mc::CurrentMove					"In ricerca di questa mossa"
::application::analysis::mc::TimeSearched					"Tempo di ricerca"
::application::analysis::mc::SearchDepth					"Profondità di ricerca in semimosse (profondità di ricerca selettiva)"
::application::analysis::mc::IllegalPosition				"Posizione illegale - Impossibile analizzare"
::application::analysis::mc::IllegalMoves					"Mosse illegali nella partita - Impossibile analizzare"
::application::analysis::mc::DidNotReceivePong			"Il motore non sta rispondendo al comando \"ping\" - Motore terminato"
::application::analysis::mc::SearchMateNotSupported	"This engine is not supporting search for mate." ;# NEW
::application::analysis::mc::EngineIsPausing				"This engine is currently pausing." ;# NEW
::application::analysis::mc::PressEngineButton			"Use the locomotive for starting a motor." ;# NEW
::application::analysis::mc::Stopped						"stopped" ;# NEW
::application::analysis::mc::OpponentsView				"Opponents view" ;# NEW
::application::analysis::mc::InsertMoveAsComment		"Insert move as comment" ;# NEW
::application::analysis::mc::SetupEvalEdges				"Setup evaluation edges" ;# NEW
::application::analysis::mc::InvalidEdgeValues			"Invalid edge values." ;# NEW
::application::analysis::mc::MustBeAscending				"The values must be strictly ascending as in the examples." ;# NEW
::application::analysis::mc::StartMotor					"Start motor" ;# NEW
::application::analysis::mc::StartOfMotorFailed			"Start of motor failed"
::application::analysis::mc::WineIsNotInstalled			"'Wine' is not (properly) installed" ;# NEW

::application::analysis::mc::LinesPerVariation			"Varianti per valutazione"
::application::analysis::mc::BestFirstOrder				"Ordina per valutazione"
::application::analysis::mc::Engine							"Motore"

# Note for translators: don't use more than 4 characters
::application::analysis::mc::Ply								"ply"
::application::analysis::mc::Seconds						"sec"
::application::analysis::mc::Minutes						"min"

::application::analysis::mc::Show(more)					"Show more" ;# NEW
::application::analysis::mc::Show(less)					"Show less" ;# NEW

::application::analysis::mc::Status(checkmate)			"%s è scacco matto"
::application::analysis::mc::Status(stalemate)			"%s è stallo"
::application::analysis::mc::Status(threechecks)		"%s ha preso tre scacchi"
::application::analysis::mc::Status(losing)				"%s ha perso tutti i pezzi"
::application::analysis::mc::Status(check)				"%s is in check" ;# NEW

::application::analysis::mc::NotSupported(standard)	"Questo motore non supporta scacchi standard."
::application::analysis::mc::NotSupported(chess960)	"Questo motore non supporta scacchi 960."
::application::analysis::mc::NotSupported(variant)		"Questo motore non supporta la variante '%s'."
::application::analysis::mc::NotSupported(analyze)		"Questo motore non ha una modalità di analisi."

::application::analysis::mc::Signal(stopped)				"Motore interrotto da segnale."
::application::analysis::mc::Signal(resumed)				"Motore riattivato da segnale."
::application::analysis::mc::Signal(killed)				"Motore in crash o terminato da segnale."
::application::analysis::mc::Signal(crashed)				"Il motore è crashato."
::application::analysis::mc::Signal(closed)				"Il motore ha terminato la connessione."
::application::analysis::mc::Signal(terminated)			"Motore interrotto con codice di errore %s."

::application::analysis::mc::Add(move)						"Append move" ;# NEW
::application::analysis::mc::Add(seq)						"Append variation" ;# NEW
::application::analysis::mc::Add(var)						"Add move as new variation" ;# NEW
::application::analysis::mc::Add(line)						"Add variation" ;# NEW
::application::analysis::mc::Add(all)						"Add all variations" ;# NEW
::application::analysis::mc::Add(merge)					"Merge variation" ;# NEW
::application::analysis::mc::Add(incl)						"Merge all variations"

### gametable ##########################################################
::gamestable::mc::DeleteGame				"Segna partita come eliminata"
::gamestable::mc::UndeleteGame			"Ripristina questa partita"
::gamestable::mc::EditGameFlags			"Modifica identificatori partita"
::gamestable::mc::Custom					"Personalizza"

::gamestable::mc::Monochrome				"Monocroma"
::gamestable::mc::Transparent				"Trasparente"
::gamestable::mc::Relief					"Rilievo"
::gamestable::mc::ShowIdn					"Mostra numero posizione di Scacchi 960"
::gamestable::mc::Icons						"Icone"
::gamestable::mc::Abbreviations			"Abbreviazioni"

::gamestable::mc::SortAscending			"Ordina (ascendente)"
::gamestable::mc::SortDescending			"Ordina (discendente)"
::gamestable::mc::SortOnAverageElo		"Ordina su Elo medio (discendente)"
::gamestable::mc::SortOnAverageRating	"Ordina su punteggio medio (discendente)"
::gamestable::mc::SortOnDate				"Ordina su data (discendente)"
::gamestable::mc::SortOnNumber			"Ordina su numero partita (ascendente)"
::gamestable::mc::ReverseOrder			"Inverti ordine"
::gamestable::mc::CancelSort				"Elimina ordinamento"
::gamestable::mc::NoMoves					"Nessuna mossa"
::gamestable::mc::NoMoreMoves				"Nessuna altra mossa"
::gamestable::mc::WhiteRating				"Punteggio Bianco"
::gamestable::mc::BlackRating				"Punteggio Nero"

::gamestable::mc::Flags						"Identificatori"
::gamestable::mc::PGN_CountryCode		"Codice paese PGN"
::gamestable::mc::ISO_CountryCode		"Codice paese ISO"
::gamestable::mc::ExcludeElo				"Escludi Elo"
::gamestable::mc::IncludePlayerType		"Includi tipo giocatore"
::gamestable::mc::ShowTournamentTable	"Tabella torneo"

::gamestable::mc::Long						"Lungo"
::gamestable::mc::Short						"Corto"
::gamestable::mc::IncludeVars				"Include Variations" ;# NEW

::gamestable::mc::Accel(browse)			"W"
::gamestable::mc::Accel(overview)		"O"
::gamestable::mc::Accel(tourntable)		"T"
::gamestable::mc::Accel(openurl)			"U"
::gamestable::mc::Space						"Spazio"

::gamestable::mc::F_Number					"#"
::gamestable::mc::F_White					"Bianco"
::gamestable::mc::F_Black					"Nero"
::gamestable::mc::F_Event					"Evento"
::gamestable::mc::F_Site					"Sito"
::gamestable::mc::F_Date					"Data"
::gamestable::mc::F_Result					"Risultato"
::gamestable::mc::F_Round					"Turno"
::gamestable::mc::F_Annotator				"Commentatore"
::gamestable::mc::F_Length					"Lunghezza"
::gamestable::mc::F_Termination			"Terminazione"
::gamestable::mc::F_EventMode				"Modalità"
::gamestable::mc::F_Eco						"ECO"
::gamestable::mc::F_Flags					"Identificatori"
::gamestable::mc::F_Material				"Materiale"
::gamestable::mc::F_Acv						"ACV"
::gamestable::mc::F_Idn						"960"
::gamestable::mc::F_Position				"Posizione"
::gamestable::mc::F_MoveList				"Move List" ;# NEW
::gamestable::mc::F_EventDate				"Data Evento"
::gamestable::mc::F_EventType				"Tipo Ev."
::gamestable::mc::F_Promotion				"Promozione"
::gamestable::mc::F_UnderPromo			"Sotto-Promozione"
::gamestable::mc::F_StandardPos			"Posizione Standard"
::gamestable::mc::F_Chess960Pos			"9"
::gamestable::mc::F_Opening				"Apertura"
::gamestable::mc::F_Variation				"Variante"
::gamestable::mc::F_Subvariation			"Sottovariante"
::gamestable::mc::F_Overview				"Panoramica"
::gamestable::mc::F_Key						"Codice ECO interno"

::gamestable::mc::T_Number					"Numero"
::gamestable::mc::T_Acv						"Annotazioni / Commenti / Varianti"
::gamestable::mc::T_WhiteRatingType		"Tipo punteggio Bianco"
::gamestable::mc::T_BlackRatingType		"Tipo punteggio Nero"
::gamestable::mc::T_WhiteCountry			"Federazione Bianco"
::gamestable::mc::T_BlackCountry			"Federazione Nero"
::gamestable::mc::T_WhiteTitle			"Titolo del bianco"
::gamestable::mc::T_BlackTitle			"Titolo del nero"
::gamestable::mc::T_WhiteType				"Tipo bianco"
::gamestable::mc::T_BlackType				"Tipo nero"
::gamestable::mc::T_WhiteSex				"Genere bianco"
::gamestable::mc::T_BlackSex				"Genere nero"
::gamestable::mc::T_EventCountry			"Nazione dell'evento"
::gamestable::mc::T_EventType				"Tipo dell'evento"
::gamestable::mc::T_Chess960Pos			"Posizione Scacchi 960"
::gamestable::mc::T_Deleted				"Eliminate"
::gamestable::mc::T_Changed				"Modificato"
::gamestable::mc::T_Added					"Aggiunto"
::gamestable::mc::T_EngFlag				"Identificatore Lingua Inglese"
::gamestable::mc::T_OthFlag				"Identificatori per Altre Lingue"
::gamestable::mc::T_Idn						"Numero Posizione Scacchi 960"
::gamestable::mc::T_Annotations			"Annotazioni"
::gamestable::mc::T_Comments				"Commenti"
::gamestable::mc::T_Variations			"Varianti"
::gamestable::mc::T_TimeMode				"Cadenza"

::gamestable::mc::P_Name					"Nome"
::gamestable::mc::P_FideID					"Fide ID"
::gamestable::mc::P_Rating					"Punteggio"
::gamestable::mc::P_RatingType			"Tipo punteggio"
::gamestable::mc::P_Country				"Nazione"
::gamestable::mc::P_Title					"Titolo"
::gamestable::mc::P_Type					"Tipo"
::gamestable::mc::P_Sex						"Genere"

::gamestable::mc::G_Player					"Player data"
::gamestable::mc::G_Event					"Event data"
::gamestable::mc::G_Game					"Game information"
::gamestable::mc::G_Opening				"Opening information"
::gamestable::mc::G_Flags					"Flags"
::gamestable::mc::G_Notation				"Notation"
::gamestable::mc::G_Internal				"Internal"

::gamestable::mc::EventType(game)		"Partita"
::gamestable::mc::EventType(match)		"Match"
::gamestable::mc::EventType(tourn)		"Torneo"
::gamestable::mc::EventType(swiss)		"Svizzero"
::gamestable::mc::EventType(team)		"Squadre"
::gamestable::mc::EventType(k.o.)		"K.O."
::gamestable::mc::EventType(simul)		"Simul"
::gamestable::mc::EventType(schev)		"Schev"

::gamestable::mc::PlayerType(human)		"Umano"
::gamestable::mc::PlayerType(program)	"Computer"

::gamestable::mc::GameFlags(w)			"Apertura Bianco"
::gamestable::mc::GameFlags(b)			"Apertura Nero"
::gamestable::mc::GameFlags(m)			"Mediogioco"
::gamestable::mc::GameFlags(e)			"Finale"
::gamestable::mc::GameFlags(N)			"Novità"
::gamestable::mc::GameFlags(p)			"Struttura pedonale"
::gamestable::mc::GameFlags(T)			"Tattica"
::gamestable::mc::GameFlags(K)			"Lato di re"
::gamestable::mc::GameFlags(Q)			"Lato di donna"
::gamestable::mc::GameFlags(!)			"Brillante"
::gamestable::mc::GameFlags(?)			"Svista"
::gamestable::mc::GameFlags(U)			"Utente"
::gamestable::mc::GameFlags(*)			"Miglior partita"
::gamestable::mc::GameFlags(D)			"Partita decisiva"
::gamestable::mc::GameFlags(G)			"Partita modello"
::gamestable::mc::GameFlags(S)			"Strategia"
::gamestable::mc::GameFlags(^)			"Attacco"
::gamestable::mc::GameFlags(~)			"Sacrificio"
::gamestable::mc::GameFlags(=)			"Difesa"
::gamestable::mc::GameFlags(M)			"Materiale"
::gamestable::mc::GameFlags(P)			"Gioco di pezzi"
::gamestable::mc::GameFlags(t)			"Svista tattica"
::gamestable::mc::GameFlags(s)			"Svista strategica"
::gamestable::mc::GameFlags(C)			"Arrocco illegale"
::gamestable::mc::GameFlags(I)			"Mossa illegale"
::gamestable::mc::GameFlags(X)			"Invalid Move" ;# NEW

### playertable ########################################################
::playertable::mc::F_LastName					"Cognome"
::playertable::mc::F_FirstName				"Nome"
::playertable::mc::F_FideID					"ID Fide"
::playertable::mc::F_DSBID						"ID DSB"
::playertable::mc::F_ECFID						"ID ECF"
::playertable::mc::F_ICCFID					"ID ICCF"
::playertable::mc::F_Title						"Titolo"
::playertable::mc::F_Frequency				"Frequenza"

::playertable::mc::T_Federation				"Federazione"
::playertable::mc::T_NativeCountry			"Paese di nascita"
::playertable::mc::T_RatingType				"Tipo punteggio"
::playertable::mc::T_Type						"Tipo"
::playertable::mc::T_Sex						"Genere"
::playertable::mc::T_PlayerInfo				"Identificatore informazioni" ;# "Info Flag" Identificatore informazioni o informazioni sull'identificatore?

::playertable::mc::Find							"Cerca"
::playertable::mc::Options						"Opzioni"
::playertable::mc::StartSearch				"Comincia ricerca"
::playertable::mc::ClearEntries				"Pulisci form"
::playertable::mc::NotFound					"Nessun risultato."
::playertable::mc::EnablePlayerBase			"Usa Database Giocatore"
::playertable::mc::DisablePlayerBase		"Disable use of player base" ;# NEW
::playertable::mc::TooltipRating				"Rating: %s" ;# NEW

::playertable::mc::Name							"Nome"
::playertable::mc::HighestRating				"Punteggio più alto"
::playertable::mc::MostRecentRating			"Punteggio più recente"
::playertable::mc::DateOfBirth				"Data di nascita"
::playertable::mc::DateOfDeath				"Data di morte"

::playertable::mc::ShowPlayerCard			"Mostra Informazioni Giocatore..."

### sitetable ##########################################################
::sitetable::mc::FindSite	"Search Site" ;# NEW
::sitetable::mc::T_Country	"Country" ;# NEW

### eventtable #########################################################
::eventtable::mc::Attendance	"Frequenza di partecipazione"
::eventtable::mc::FindEvent	"Search Event Name" ;# NEW

### player dictionary ##################################################
::playerdict::mc::PlayerDictionary		"Dizionario Giocatore"
::playerdict::mc::PlayerFilter			"Filtri Giocatore"
::playerdict::mc::OrganizationID			"Organization ID" ;# NEW
::playerdict::mc::Count						"Conteggio"
::playerdict::mc::Ignore					"Ignora"
::playerdict::mc::FederationID			"ID della Federazione"
::playerdict::mc::Ratings					"Punteggi"
::playerdict::mc::Titles					"Titoli"
::playerdict::mc::None						"Nessuno"
::playerdict::mc::Operation				"Operazione"
::playerdict::mc::Awarded					"Awarded" ;# NEW
::playerdict::mc::RangeOfYears			"Range of years" ;# NEW
::playerdict::mc::SearchPlayerName		"Search Player Name" ;# NEW
::playertable::mc::HelpPatternMatching	"Help: Pattern Matching" ;# NEW

::playerdict::mc::AgeClass(unrestricted)	"Unrestricted" ;# NEW
::playerdict::mc::AgeClass(junior)			"Junior" ;# NEW
::playerdict::mc::AgeClass(senior)			"Senior" ;# NEW

::playerdict::mc::Champions(world)	"World Champions" ;# NEW
::playerdict::mc::Champions(eu)		"European Champions" ;# NEW
::playerdict::mc::Champions(nat)		"National Champions" ;# NEW

::playerdict::mc::T_Ranking			"Ranking" ;# NEW
::playerdict::mc::T_Trophy				"Trophies" ;# NEW

# see tcl/lang/CHAPIONS.txt how this will be constructed
::playerdict::mc::ChessChampion	"%sex% %mode% %under%%age% %region% %champion% %where%" ;# NEW
::playerdict::mc::Sex(f)			"Woman" ;# NEW
::playerdict::mc::Sex(m)			"" ;# NEW
::playerdict::mc::Region(r)		"World" ;# NEW
::playerdict::mc::Region(e)		"European" ;# NEW
::playerdict::mc::Region(-)		"National" ;# NEW
::playerdict::mc::Champion(w)		"Champion" ;# NEW
::playerdict::mc::Champion(e)		"Champion" ;# NEW
::playerdict::mc::Champion(-)		"Champion" ;# NEW
::playerdict::mc::Age(j)			"Junior" ;# NEW
::playerdict::mc::Age(s)			"Senior" ;# NEW
::playerdict::mc::Age(-)			"" ;# NEW
::playerdict::mc::Mode(c)			"Correspondence" ;# NEW
::playerdict::mc::Mode(-)			"" ;# NEW
::playerdict::mc::Where				"in %country%" ;# NEW

### player-card ########################################################
::playercard::mc::PlayerCard					"Informazioni Giocatore"
::playercard::mc::Latest						"Ultimo"
::playercard::mc::Highest						"Più alto"
::playercard::mc::Minimal						"Minimo"
::playercard::mc::Maximal						"Massimo"
::playercard::mc::Win							"Vinte"
::playercard::mc::Draw							"Patte"
::playercard::mc::Loss							"Perse"
::playercard::mc::Total							"Totale"
::playercard::mc::FirstGamePlayed			"Prima partita giocata"
::playercard::mc::LastGamePlayed				"Ultima partita giocata"
::playercard::mc::WhiteMostPlayed			"Apertura più giocata da Bianco"
::playercard::mc::BlackMostPlayed			"Apertura più giocata da Nero"

::playercard::mc::OpenPlayerCard				"Apri profilo utente %s"
::playercard::mc::OpenFileCard				"Apri profilo file %s"
::playercard::mc::OpenFideRatingHistory	"Apri storia punteggio FIDE"
::playercard::mc::OpenWikipedia				"Apri biografia Wikipedia"
::playercard::mc::OpenViafCatalog			"Apri catalogo VIAF"
::playercard::mc::OpenPndCatalog				"Apri catalogo della Deutsche Nationalbibliothek"
::playercard::mc::OpenChessgames				"Collezione partite chessgames.com"
::playercard::mc::SeachIn365ChessCom		"Cerca in 365Chess.com"

### fonts ##############################################################
::font::mc::ChessBaseFontsInstalled				"Caratteri di ChessBase installati con successo."
::font::mc::ChessBaseFontsInstallationFailed	"Installazione dei caratteri di ChessBase fallita."
::font::mc::NoChessBaseFontFound					"Nessun carattere di ChessBase trovato nella cartella '%s'."
::font::mc::ChessBaseFontsAlreadyInstalled	"Caratteri di ChessBase già installati. Installa comunque?"
::font::mc::ChooseMountPoint						"Punto di montaggio della partizione di installazione Windows"
::font::mc::CopyingChessBaseFonts				"Copiando i caratteri di ChessBase"
::font::mc::CopyFile									"Copiando i file %s"
::font::mc::UpdateFontCache						"Aggiornando cache caratteri"

::font::mc::ChooseFigurineFont					"Scegli un font figurine"
::font::mc::ChooseSymbolFont						"Scegli un font per i simboli"
::font::mc::IncreaseFontSize						"Aumenta Grandezza Font"
::font::mc::DecreaseFontSize						"Diminuisci Grandezza Font"
::font::mc::DefaultFont								"Default font" ;# NEW

### gamebar ############################################################
::gamebar::mc::StartPosition					"Posizione di partenza"
::gamebar::mc::Players							"Giocatori"
::gamebar::mc::Event								"Eventi"
::gamebar::mc::Site								"Sito"
::gamebar::mc::SeparateHeader					"Separa intestazione"
::gamebar::mc::ShowActiveAtBottom			"Mostra partita attiva in basso"
::gamebar::mc::ShowPlayersOnSeparateLines			"Mostra giocatori su righe separate"
::gamebar::mc::DiscardChanges					"Questa partita è stata modificata.\n\nVuoi davvero annullare i cambiamenti fatti?"
::gamebar::mc::DiscardNewGame					"Vuoi davvero buttare via questa partita?"
::gamebar::mc::NewGameFstPart					"Nuovo"
::gamebar::mc::NewGameSndPart					"Partita"
::gamebar::mc::EnterGameNumber				"Inserisci numero partita"

::gamebar::mc::CopyThisGameToClipbase		"Copia questa partita nella Clipbase"
::gamebar::mc::CopyThisGameToClipboard		"Copia questa partita nella Clipboard (formato PGN)"
::gamebar::mc::ExportThisGame					"Esporta questa partita"
::gamebar::mc::PasteLastClipbaseGame		"Incolla ultima partita della Clipbase"
::gamebar::mc::PasteGameFrom					"Incolla partita"
::gamebar::mc::LoadGameNumber					"Carica partita numero"
::gamebar::mc::ReloadCurrentGame				"Ri-carica partita corrente"
::gamebar::mc::OriginalVersion				"Versione originale dal database"
::gamebar::mc::ModifiedVersion				"Versione modificata nell'editor partita"
::gamebar::mc::WillCopyModifiedGame			"Questa operazione copierà la partita modificata nell'editor. La versione originale non può essere copiata perché il database associato non è aperto."

::gamebar::mc::CopyGame							"Copia Partita"
::gamebar::mc::ExportGame						"Esporta Partita"
::gamebar::mc::LockGame							"Blocca Partita"
::gamebar::mc::UnlockGame						"Sblocca Partita"
::gamebar::mc::CloseGame						"Chiudi Partita"

::gamebar::mc::GameNew							"Nuova Partita"
::gamebar::mc::AddNewGame						"Aggiunti nuova partita a %s..."
::gamebar::mc::ReplaceGame						"Rimpiazza partita in %s..."
::gamebar::mc::ReplaceMoves					"Sostituisci solo mosse nella partita..."

::gamebar::mc::Tip(Antichess)					"Non ci sono scacchi né arrocchi, il re viene catturato come un pezzo qualunque"
::gamebar::mc::Tip(Suicide)					"In caso di stallo il lato con minor pezzi vince (secondo le regole di FISC)."
::gamebar::mc::Tip(Giveaway)					"In caso di stallo il lato che si trova in stallo vince (secondo le regole internazionali)"
::gamebar::mc::Tip(Losers)						"Il re si comporta come negli scacchi ortodossi e puoi vincere prendendo scacco matto o stallo."

### merge ##############################################################
::merge::mc::MergeLastClipbaseGame		"Unisci ultima partita della Clipbase"
::merge::mc::MergeGameFrom					"Unisci partita"

::merge::mc::MergeTitle						"Merge with games" ;# NEW
::merge::mc::StartFromCurrentPosition	"Comincia unione da posizione corrente"
::merge::mc::StartFromInitialPosition	"Comincia unione da posizione iniziale"
::merge::mc::NoTranspositions				"Nessuna trasposizione"
::merge::mc::IncludeTranspositions		"Includi trasposizioni"
::merge::mc::VariationDepth				"Profondità variante"
::merge::mc::VariationLength				"Maximal variation length (plies)" ;# NEW
::merge::mc::UpdatePreview					"Update preview" ;# NEW
::merge::mc::SelectedGame					"Selected Game" ;# NEW
::merge::mc::SaveAs							"Save as new game" ;# NEW
::merge::mc::Save								"Merge into game" ;# NEW
::merge::mc::GameisLocked					"Game is locked by Merge-Dialog" ;# NEW

::merge::mc::AlreadyInUse					"Merge dialog is already in use with game #%d." ;# NEW
::merge::mc::AlreadyInUseDetail			"Please finish merge of this game before merging into another game. This means you have to switch to game #%d for continuing." ;# NEW
::merge::mc::CannotMerge					"Cannot merge games with different variants." ;# NEW

### validate ###########################################################
::validate::mc::Unlimited	"illimitato"

### browser ############################################################
::browser::mc::BrowseGame			"Sfoglia Partita"
::browser::mc::StartAutoplay		"Inizia Autoplay"
::browser::mc::StopAutoplay		"Ferma Autoplay"
::browser::mc::GoForward			"Vai alla prossima mossa"
::browser::mc::GoBackward			"Vai alla mossa precedente"
::browser::mc::GoForwardFast		"Vai avanti per alcune mosse"
::browser::mc::GoBackFast			"Vai indietro per alcune mosse"
::browser::mc::GotoStartOfGame	"Vai all'inizio della partita"
::browser::mc::GotoEndOfGame		"Vai alla fine della partita"
::browser::mc::IncreaseBoardSize	"Aumenta grandezza scacchiera"
::browser::mc::DecreaseBoardSize	"Diminuisci grandezza scacchiera"
::browser::mc::MaximizeBoardSize	"Ingrandisci dimensione scacchiera"
::browser::mc::MinimizeBoardSize	"Minimizza dimensione scacchiera"
::browser::mc::LoadPrevGame		"Carica partita precedente"
::browser::mc::LoadNextGame		"Carica partita successiva"
::browser::mc::HandicapGame      "Handicap game" ;# NEW

::browser::mc::GotoGame(first)  	"Vai alla prima partita"
::browser::mc::GotoGame(last)   	"Vai all'ultima partita"
::browser::mc::GotoGame(next)		"Vai alla prossima partita"
::browser::mc::GotoGame(prev)		"Vai alla partita precedente"

::browser::mc::LoadGame				"Carica partita nell'editor"
::browser::mc::ReloadGame			"Ricarica partita"
::browser::mc::MergeGame			"Unisci partita"

::browser::mc::IllegalMove			"Mossa illegale"
::browser::mc::NoCastlingRights	"Impossibile arroccare"

### overview ###########################################################
::overview::mc::Overview				"Panoramica"
::overview::mc::RotateBoard			"Ruota scacchiera"
::overview::mc::AcceleratorRotate	"R"

### encoding ###########################################################
::encoding::mc::AutoDetect				"auto-rilevazione"

::encoding::mc::Encoding				"Codifica"
::encoding::mc::Description			"Descrizione"
::encoding::mc::Languages				"Lingue (Fonts)"
::encoding::mc::UseAutoDetection		"Usa rilevazione automatica"
::encoding::mc::AllLanguages			"Tutte le lingue"

::encoding::mc::ChooseEncodingTitle	"Scegli codifica"

::encoding::mc::CurrentEncoding		"Codifica attuale:"
::encoding::mc::DefaultEncoding		"Codifica di default:"
::encoding::mc::SystemEncoding		"Codifica di sistema:"

### setup ##############################################################
::setup::mc::Position(Chess960)	"Posizione Scacchi 960"
::setup::mc::Position(Symm960)	"Posizione Scacchi 960 Simmetrica"
::setup::mc::Position(Shuffle)	"Posizione Scacchi Shuffle"

### setup board ########################################################
::setup::position::mc::SetStartPosition		"Imposta posizione iniziale"
::setup::position::mc::UsePreviousPosition	"Usa posizione precedente"

::setup::board::mc::SetStartBoard				"Imposta scacchiera iniziale"
::setup::board::mc::SideToMove					"Lato col tratto"
::setup::board::mc::Castling						"Arrocco"
::setup::board::mc::MoveNumber					"Numero mossa"
::setup::board::mc::EnPassantFile				"En passant"
::setup::board::mc::HalfMoves						"Tempo per semi-mossa"
::setup::board::mc::StartPosition				"Posizione iniziale"
::setup::board::mc::Fen								"FEN"
::setup::board::mc::Promoted						"Promosso"
::setup::board::mc::Holding						"Pezzi in mano"
::setup::board::mc::ChecksGiven					"Scacchi Dati"
::setup::board::mc::Clear							"Pulisci"
::setup::board::mc::CopyFen						"Copia FEN a clipboard"
::setup::board::mc::Shuffle						"Mischia..."
::setup::board::mc::FICSPosition					"FICS Posizione Iniziale..."
::setup::board::mc::StandardPosition			"Posizione Standard"
::setup::board::mc::Chess960Castling			"Arrocco da Scacchi 960"
::setup::board::mc::TooManyPiecesInHolding	"one extra piece|%d extra pieces" ;# NEW
::setup::board::mc::TooFewPiecesInHolding		"one piece is missing|%d pieces are missing" ;# NEW

::setup::board::mc::ChangeToFormat(xfen)		"Converti a formato X-Fen"
::setup::board::mc::ChangeToFormat(shredder)	"Converti a formato Shredder"

::setup::board::mc::Error(InvalidFen)							"FEN non valido."
::setup::board::mc::Error(EmptyBoard)							"La scacchiera è vuota"
::setup::board::mc::Error(NoWhiteKing)							"Manca il re bianco."
::setup::board::mc::Error(NoBlackKing)							"Manca il re nero."
::setup::board::mc::Error(BothInCheck)							"Entrambi i re sono sotto scacco."
::setup::board::mc::Error(OppositeCheck)						"Il lato senza tratto è sotto scacco."
::setup::board::mc::Error(TooManyWhitePawns)					"Troppi pedoni bianchi." ;# NEW changed a bit
::setup::board::mc::Error(TooManyBlackPawns)					"Troppi pedoni neri." ;# NEW changed a bit
::setup::board::mc::Error(TooManyWhitePieces)				"Troppi pezzi bianchi." ;# NEW changed a bit
::setup::board::mc::Error(TooManyBlackPieces)				"Troppi pezzi neri." ;# NEW changed a bit
::setup::board::mc::Error(PawnsOn18)							"Pedone sulla prima o ottava traversa."
::setup::board::mc::Error(TooManyKings)						"Ci sono più di due re."
::setup::board::mc::Error(TooManyWhite)						"Troppi pezzi bianchi." ;# NEW changed a bit
::setup::board::mc::Error(TooManyBlack)						"Troppi pezzi neri." ;# NEW changed a bit
::setup::board::mc::Error(BadCastlingRights)					"Cattivi diritti di arrocco."
::setup::board::mc::Error(InvalidCastlingRights)			"Colonna della torre di arrocco non ragionevole."
::setup::board::mc::Error(InvalidCastlingFile)				"Colonna di arrocco non valida."
::setup::board::mc::Error(AmbiguousCastlingFyles)			"Per arroccare bisogna disambiguare la colonna della torre (è possibile siano messe male)."
::setup::board::mc::Error(InvalidEnPassant)					"Colonna di en passant non ragionevole."
::setup::board::mc::Error(MultiPawnCheck)						"Due o più pedoni danno scacco."
::setup::board::mc::Error(TripleCheck)							"Tre o più pezzi danno scacco."

::setup::board::mc::Error(OppositeLosing)						"Side not to move has no pieces." ;# NEW

::setup::board::mc::Error(TooManyPawnsPlusPromoted)		"Sum of pawns and promoted pieces is too large." ;# NEW
::setup::board::mc::Error(TooManyPiecesMinusPromoted)		"Sum of pieces on board (incl. King, but excl. promoted) is too large." ;# NEW
::setup::board::mc::Error(TooManyPiecesInHolding)			"Too many pieces n holding." ;# NEW
::setup::board::mc::Error(TooManyWhiteQueensInHolding)	"Too many white queens in holding." ;# NEW
::setup::board::mc::Error(TooManyBlackQueensInHolding)	"Too many black queens in holding." ;# NEW
::setup::board::mc::Error(TooManyWhiteRooksInHolding)		"Too many white rooks in holding." ;# NEW
::setup::board::mc::Error(TooManyBlackRooksInHolding)		"Too many black rooks in holding." ;# NEW
::setup::board::mc::Error(TooManyWhiteBishopsInHolding)	"Too many white bishops in holding." ;# NEW
::setup::board::mc::Error(TooManyBlackBishopsInHolding)	"Too many black bishops in holding." ;# NEW
::setup::board::mc::Error(TooManyWhiteKnightsInHolding)	"Too many white knights in holding." ;# NEW
::setup::board::mc::Error(TooManyBlackKnightsInHolding)	"Too many black knights in holding." ;# NEW
::setup::board::mc::Error(TooManyWhitePawnsInHolding)		"Too many white pawns in holding." ;# NEW
::setup::board::mc::Error(TooManyBlackPawnsInHolding)		"Too many black pawns in holding." ;# NEW
::setup::board::mc::Error(TooManyPromotedPieces)			"Too many pieces marked as promoted." ;# NEW
::setup::board::mc::Error(TooFewPromotedPieces)				"Too few pieces marked as promoted." ;# NEW
::setup::board::mc::Error(TooManyPromotedWhitePieces)		"Too many white pieces marked as promoted." ;# NEW
::setup::board::mc::Error(TooManyPromotedBlackPieces)		"Too many black pieces marked as promoted." ;# NEW
::setup::board::mc::Error(TooFewPromotedQueens)				"Too few queens marked as promoted." ;# NEW
::setup::board::mc::Error(TooFewPromotedRooks)				"Too few rooks marked as promoted." ;# NEW
::setup::board::mc::Error(TooFewPromotedBishops)			"Too few bishops marked as promoted." ;# NEW
::setup::board::mc::Error(TooFewPromotedKnights)			"Too few knights marked as promoted." ;# NEW

::setup::board::mc::Error(IllegalCheckCount)					"Unreasonable check count." ;# NEW (Three-check Chess)

::setup::board::mc::Warning(TooFewPiecesInHolding)			"Troppi pochi pezzi segnati come promossi. Sei sicuro che vada bene?"
::setup::board::mc::Warning(CastlingWithoutRook)			"Hai segnato il diritto di arroccare, ma almeno una torre di arrocco manca. Questo può succedere solo in partite con handicap. Sei sicuro che ci sia la possibilità di arroccare?"
::setup::board::mc::Warning(UnsupportedVariant)				"La posizione è una posizione iniziale ma non di Scacchi Mischiati - Shuffle Chess. Sei sicuro?"

### import #############################################################
::import::mc::ImportingFile(pgn)					"Importando file PGN"
::import::mc::ImportingFile(db)					"Importando database"
::import::mc::Line									"Linea"
::import::mc::Column									"Colonna"
::import::mc::GameNumber							"Partita"
::import::mc::ImportedGames						"%s partite importate"
::import::mc::NoGamesImported						"Nessuna partita importata"
::import::mc::FileIsEmpty							"Il file è probabilmente vuoto"
::import::mc::DatabaseImport						"Importa Database"
::import::mc::ImportPgnGame						"Importa partita in PGN"
::import::mc::ImportPgnVariation					"Importa variante in PGN"
::import::mc::ImportOK								"Testo PGN importato senza errori o avvertimenti."
::import::mc::ImportAborted						"Importazione interrotta."
::import::mc::TextIsEmpty							"Testo PGN vuoto."
::import::mc::AbortImport							"Annulla importazione PGN?"
::import::mc::UnsupportedVariantRejected		"Variante non supportata '%s' rifiutata"
::import::mc::Accepted								"accettato"
::import::mc::Rejected								"rifiutato"
::import::mc::ImportDialogAlreadyOpen			"Import dialog for this game is already open." ;# NEW

::import::mc::DifferentEncoding					"La codifica selezionata %src non corrisponde alla codifica del file %dst."
::import::mc::DifferentEncodingDetails			"La ricodifica del database non avrà più successo dopo questa azione."
::import::mc::CannotDetectFigurineSet			"Impossibile auto-rilevare un set figurine adatto."
::import::mc::TryAgainWithEnglishSet			"Riprova con notazione inglesi?"
::import::mc::TryAgainWithEnglishSetDetail	"Potrebbe aiutare usare la notazione inglese in quanto è lo standard del formato PGN"
::import::mc::CheckImportResult					"Per favore controlla che un set figurine adatto sia stato rilevato: %s."
::import::mc::CheckImportResultDetail			"In rari casi l'auto-rilevamento fallisce per ambiguità."

::import::mc::EnterOrPaste							"Inserisci o incolla un PGN %s nel campo in alto.\nErrori legati all'importazione di %s saranno mostrati qui."
::import::mc::EnterOrPaste-Game					"partita"
::import::mc::EnterOrPaste-Variation			"variante"

::import::mc::State(UnsupportedVariant)		"Variante non supportata rifiutata"
::import::mc::State(DecodingFailed)				"La decodifica di questa partita non è stata possibile"
::import::mc::State(TooManyGames)				"Troppe partite nel database (interrotto)"
::import::mc::State(FileSizeExceeded)			"La grandezza massima del file (2GB) sarà superata (interrotto)"
::import::mc::State(GameTooLong)					"Partita troppo lunga (saltata)"
::import::mc::State(TooManyPlayerNames)		"Troppi nomi di giocatori nel database (interrotto)"
::import::mc::State(TooManyEventNames)			"Troppi nomi di eventi nel database (interrotto)"
::import::mc::State(TooManySiteNames)			"Troppi nomi di siti nel database (interrotto)"
::import::mc::State(TooManyRoundNames)			"Troppi turni nel database"
::import::mc::State(TooManyAnnotatorNames)	"Troppi nomi di commentatori nel database (interrotto)"
::import::mc::State(TooManySourceNames)		"Troppi nomi fonte nel database (interrotto)"

::import::mc::Warning(MissingWhitePlayerTag)				"Manca giocatore bianco"
::import::mc::Warning(MissingBlackPlayerTag)				"Manca giocatore nero"
::import::mc::Warning(MissingPlayerTags)					"Mancano i giocatori"
::import::mc::Warning(MissingResult)						"Manca risultato (alla fine della selezione mosse)"
::import::mc::Warning(MissingResultTag)					"Manca risultato (nella sezione tag)"
::import::mc::Warning(InvalidRoundTag)						"Tag turno non valida"
::import::mc::Warning(InvalidResultTag)					"Tag risultato non valida"
::import::mc::Warning(InvalidDateTag)						"Tag data non valida"
::import::mc::Warning(InvalidEventDateTag)				"Tag data evento non valida"
::import::mc::Warning(InvalidTimeModeTag)					"Tag cadenza non valida"
::import::mc::Warning(InvalidEcoTag)						"Tag ECO non valida"
::import::mc::Warning(InvalidTagName)						"Tag nome non valida (ignorata)"
::import::mc::Warning(InvalidCountryCode)					"Codice paese non valido"
::import::mc::Warning(InvalidRating)						"Valore punteggio non valido"
::import::mc::Warning(InvalidNag)							"NAG non valido"
::import::mc::Warning(BraceSeenOutsideComment)			"\"\}\" visto fuori da un commento nella partita (ignorato)"
::import::mc::Warning(MissingFen)							"Manca FEN (tag variante sarà ignorata)"
::import::mc::Warning(FixedInvalidFen)						"Castle rights in FEN have been fixed" ;# NEW
::import::mc::Warning(UnknownEventType)					"Tipo di evento sconosciuto"
::import::mc::Warning(UnknownTitle)							"Titolo sconosciuto (ignorato)"
::import::mc::Warning(UnknownPlayerType)					"Tipo di giocatore sconosciuto (ignorato)"
::import::mc::Warning(UnknownSex)							"Genere sconosciuto (ignorato)"
::import::mc::Warning(UnknownTermination)					"Ragione del termine sconosciuta"
::import::mc::Warning(UnknownMode)							"Modalità sconosciuta"
::import::mc::Warning(RatingTooHigh)						"Elo troppo alto (ignorato)"
::import::mc::Warning(EncodingFailed)						"Character decoding failed"
::import::mc::Warning(TooManyNags)							"Troppi NAG (ultimo ignorato)"
::import::mc::Warning(IllegalCastling)						"Arrocco illegale"
::import::mc::Warning(IllegalMove)							"Mossa illegale"
::import::mc::Warning(CastlingCorrection)					"Correzione arrocco"
::import::mc::Warning(ResultDidNotMatchHeaderResult)	"Il risultato non corrisponde alle informazioni fornite"
::import::mc::Warning(ValueTooLong)							"Il valore del tag è troppo lungo e sarà interrotto a 255 caratteri"
::import::mc::Warning(NotSuicideNotGiveaway)				"Dato l'esito della partita la variante non è Vinciperdi."
::import::mc::Warning(VariantChangedToGiveaway)			"Dato l'esito della partita la variante è stata cambiata a Vinciperdi"
::import::mc::Warning(VariantChangedToSuicide)			"Dato l'esito della partita la variante è stata cambiata a Vinciperdi"
::import::mc::Warning(ResultCorrection)					"Dato l'esito della partita è stata effettuata una correzione del risultato"
::import::mc::Warning(MaximalErrorCountExceeded)		"Numero massimo di errori superato; non saranno riportati altri errori analoghi"
::import::mc::Warning(MaximalWarningCountExceeded)		"Numero massimo di avvertimenti superato; non saranno riportati altri avvertimenti analoghi"

::import::mc::Error(InvalidToken)							"Token non valido"
::import::mc::Error(InvalidMove)								"Mossa non valida"
::import::mc::Error(UnexpectedSymbol)						"Simbolo inatteso"
::import::mc::Error(UnexpectedEndOfInput)					"Fine dell'inserimento inatteso"
::import::mc::Error(UnexpectedResultToken)				"Token del risultato inatteso"
::import::mc::Error(UnexpectedTag)							"Tag all'interno della partita inattesa"
::import::mc::Error(UnexpectedEndOfGame)					"Fine partita inattesa (manca il risultato)"
::import::mc::Error(UnexpectedCastling)					"Arrocco inaspettato (non permesso in questa variante degli scacchi"
::import::mc::Error(ContinuationsNotSupported)			"'Continuazioni' non supportate"
::import::mc::Error(TagNameExpected)						"Errore di sintassi: serve un nome per il Tag"
::import::mc::Error(TagValueExpected)						"Errore di sintassi: serve un valore per il Tag"
::import::mc::Error(InvalidFen)								"FEN non valido"
::import::mc::Error(UnterminatedString)					"Stringa indeterminata"
::import::mc::Error(UnterminatedVariation)				"Variante indeterminata"
::import::mc::Error(SeemsNotToBePgnText)					"Potrebbe non essere un testo PGN"
::import::mc::Error(AbortedDueToInternalError)			"Annullato per errore interno"
::import::mc::Error(AbortedDueToIoError)					"Annullato per errore di lettura/scrittura"
::import::mc::Error(UserHasInterrupted)					"L'utente ha interrotto" ;#User has interrupted?


### export #############################################################
::export::mc::FileSelection				"&Selezione File"
::export::mc::OptionsSetup					"&Opzioni"
::export::mc::PageSetup						"Set&up pagina"
::export::mc::DiagramSetup					"&Diagram Setup"
::export::mc::StyleSetup					"Sti&le"
::export::mc::EncodingSetup				"Codi&fica"
::export::mc::TagsSetup						"&Tags"
::export::mc::NotationSetup				"&Notazione"
::export::mc::AnnotationSetup				"&Annotazione"
::export::mc::CommentsSetup				"&Commenti"

::export::mc::Visibility					"Visibilità"
::export::mc::HideDiagrams					"Nascondi Diagrammi"
::export::mc::AllFromWhitePersp			"Tutto dalla prospettiva del Bianco"
::export::mc::AllFromBlackPersp			"Tutto dalla prospettiva del Nero"
::export::mc::ShowCoordinates				"Mostra coordinate"
::export::mc::ShowSideToMove				"Mostra lato con il tratto"
::export::mc::ShowArrows					"Mostra Frecce"
::export::mc::ShowMarkers					"Mostra Codici"
::export::mc::Layout							"Layout"
::export::mc::PostscriptSpecials			"Speciali Postscript"
::export::mc::BoardSize						"Grandezza Scacchiera"

::export::mc::Short							"Corto"
::export::mc::Long							"Lungo"
::export::mc::Algebraic						"Algebrico"
::export::mc::Correspondence				"Corrispondenza"
::export::mc::Telegraphic					"Telegrafico"
::export::mc::FontHandling					"Gestione caratteri"
::export::mc::DiagramStyle					"Stile Diagramma"
::export::mc::UseImagesForDiagram		"Usa immagini per la generazione dei diagrammi"
::export::mc::EmebedTruetypeFonts		"Inserisci fond TrueType"
::export::mc::UseBuiltinFonts				"Usa caratteri nativi"
::export::mc::SelectExportedTags			"Selezione dei tag per l'esportazione"
::export::mc::ExcludeAllTags				"Escludi tutti i tag"
::export::mc::IncludeAllTags				"Includi tutti i tag"
::export::mc::ExtraTags						"Tutti gli altri tag"
::export::mc::NoComments					"Nessun commento"
::export::mc::AllLanguages					"All languages" ;# NEW
::export::mc::SelectLanguages				"Selected languages" ;# NEW
::export::mc::LanguageSelection			"Selezione Lingua"
::export::mc::MapTo							"Mappa NAG a" ;# (map to)
::export::mc::MapNagsToComment			"Mappa annotazioni (NAG) a commenti" ;# (map annotations to comments)
::export::mc::UnusualAnnotation			"Annotazione Inusuali"
::export::mc::AllAnnotation				"Tutte le annotazioni"
::export::mc::UseColumnStyle				"Usa stile colonna"
::export::mc::MainlineStyle				"Stile Linea Principale"
::export::mc::HideVariations				"Nascondi Varianti"
::export::mc::GameDoesNotHaveComments	"This game does not contain comments." ;# NEW

::export::mc::LanguageSelectionDescr	"The checkbox (right side from combo box) has the meaning 'significant'.\n\nLanguages marked as 'significant' will always be exported.\n\nIf the game includes none of the languages marked as 'significant' then the first available language will be exported." ;# NEW

::export::mc::PdfFiles						"File PDF"
::export::mc::HtmlFiles						"File HTML"
::export::mc::TeXFiles						"File LaTeX"

::export::mc::ExportDatabase				"Esporta database"
::export::mc::ExportDatabaseVariant		"Esporta database - variante %s"
::export::mc::ExportDatabaseTitle		"Esporta Database '%s'"
::export::mc::ExportCurrentGameTitle	"Esporta Partita Corrente"
::export::mc::ExportingDatabase			"Sto esportando '%s' nel file '%s'"
::export::mc::Export							"Esporta"
::export::mc::NoGamesCopied				"Nessuna partita esportata."
::export::mc::ExportedGames				"%s partite esportate"
::export::mc::NoGamesForExport			"Nessuna partita da esportare."
::export::mc::ResetDefaults				"Torna a valori di default"
::export::mc::UnsupportedEncoding		"Impossibile usare codifica %s per documenti PDF. Devi usare una codifica alternativa."
::export::mc::DatabaseIsOpen				"Il database di destinazione '%s' è aperto, questo vuol dire che il database di destinazione sarà svuotato prima dell'inizio dell'esportazione. Esportare comunque?"
::export::mc::DatabaseIsOpenDetail		"Se invece vuoi aggiungere devi fare un Drag&Drop dentro il selezionatore di database."
::export::mc::DatabaseIsReadonly			"The destination database '%s' is already existing, and you don't have permissions for overwriting." ;# NEW
::export::mc::ExportGamesFromTo			"Esporta partite da '%src' a '%dst'"
::export::mc::IllegalRejected				"%s partite rifiutate per mosse illegali"

::export::mc::BasicStyle					"Stile base"
::export::mc::GameInfo						"Informazioni della partita"
::export::mc::GameText						"Testo della partita"
::export::mc::Moves							"Mosse"
::export::mc::MainLine						"Linea principale"
::export::mc::Variation						"Variante"
::export::mc::Subvariation					"Sottovariante"
::export::mc::Figurines						"Figurines"
::export::mc::Hyphenation					"Sillabazione"
::export::mc::None							"(nessuno)"
::export::mc::Symbols						"Simboli"
::export::mc::Comments						"Commenti"
::export::mc::Result							"Risultato"
::export::mc::Diagram						"Diagramma"
::export::mc::ColumnStyle					"Stile Colonna"

::export::mc::Paper							"Pagine"
::export::mc::Orientation					"Orientamento"
::export::mc::Margin							"Margine"
::export::mc::Format							"Formato"
::export::mc::Size							"Grandezza"
::export::mc::Custom							"Personalizza"
::export::mc::Potrait						"Ritratto"
::export::mc::Landscape						"Panoramica"
::export::mc::Justification				"Giustificazione"
::export::mc::Even							"Pareggia"
::export::mc::Columns						"Colonne"
::export::mc::One								"Uno"
::export::mc::Two								"Due"

::export::mc::DocumentStyle				"Stile Documento"
::export::mc::Article						"Articolo"
::export::mc::Report							"Report"
::export::mc::Book							"Libro"

::export::mc::FormatName(scidb)			"Scidb"
::export::mc::FormatName(scid)			"Scid"
::export::mc::FormatName(pgn)				"PGN"
::export::mc::FormatName(pdf)				"PDF"
::export::mc::FormatName(html)			"HTML"
::export::mc::FormatName(tex)				"LaTeX"
::export::mc::FormatName(ps)				"Postscript"

::export::mc::Option(pgn,include_varations)						"Esporta variante"
::export::mc::Option(pgn,include_comments)						"Esporta commenti"
::export::mc::Option(pgn,include_moveinfo)						"Esporta informazioni mossa (come commenti)"
::export::mc::Option(pgn,include_marks)							"Esporta codici (come commenti)"
::export::mc::Option(pgn,use_scidb_import_format)				"Usa formato importazione Scidb" ;# [chessbase?]
::export::mc::Option(pgn,use_chessbase_format)					"Usa formato ChessBase"
::export::mc::Option(pgn,use_strict_pgn_standard)				"Usa standard PGN"
::export::mc::Option(pgn,include_ply_count_tag)					"Scrivi tag 'PlyCount'"
::export::mc::Option(pgn,include_termination_tag)				"Scrivi tag 'Termination'"
::export::mc::Option(pgn,include_mode_tag)						"Scrivi tag 'Mode'"
::export::mc::Option(pgn,include_opening_tag)					"Scrivi tag 'Opening', 'Variation', 'Subvariation'"
::export::mc::Option(pgn,include_setup_tag)						"Scrivi tag 'Setup' (se necessario)"
::export::mc::Option(pgn,include_variant_tag)					"Scrivi tag 'Variant' (se necessario)"
::export::mc::Option(pgn,include_position_tag)					"Scrivi tag 'Position' (se necessario)"
::export::mc::Option(pgn,include_time_mode_tag)					"Scrivi tag 'TimeMode' (se necessario)"
::export::mc::Option(pgn,exclude_extra_tags)						"Escludi tag estranei"
::export::mc::Option(pgn,indent_variations)						"Indenta varianti"
::export::mc::Option(pgn,indent_comments)							"Indenta commenti"
::export::mc::Option(pgn,column_style)								"Stile Colonna (una mossa per riga)"
::export::mc::Option(pgn,symbolic_annotation_style)			"Annotazione simbolica (!, !?)"
::export::mc::Option(pgn,extended_symbolic_style)				"Espandi stile notazione simbolica (+=, +/-)"
::export::mc::Option(pgn,convert_null_moves)						"Trasforma mosse nulle a commento"
::export::mc::Option(pgn,space_after_move_number)				"Aggiungi spazio dopo numero mossa"
::export::mc::Option(pgn,shredder_fen)								"Scrivi in Shredder-FEN (default è X-FEN)"
::export::mc::Option(pgn,convert_lost_result_to_comment)		"Scrivi commento per risultato '0-0'"
::export::mc::Option(pgn,write_any_rating_as_elo)				"Scrivi qualsiasi punteggio come ELO"
::export::mc::Option(pgn,append_mode_to_event_type)			"Aggiungi modalità dopo il tipo dell'evento"
::export::mc::Option(pgn,comment_to_html)							"Scrivi commento in stile HTML"
::export::mc::Option(pgn,exclude_games_with_illegal_moves)	"Escludi partite con mosse illegali"
::export::mc::Option(pgn,use_utf8_encoding)						"Usa encoding UTF-8"

### notation ###########################################################
::notation::mc::Notation		"Notazione"

::notation::mc::MoveForm(can)	"Computer Algebraic Notation" ;# NEW also: "Coordinate Notation"
::notation::mc::MoveForm(san)	"Short Algebraic Notation" ;# NEW also "Standard Algebraic Notation"
::notation::mc::MoveForm(lan)	"Long Algebraic Notation"
::notation::mc::MoveForm(gan)	"German Short Algebraic Notation"
::notation::mc::MoveForm(man)	"Minimal Algebraic Notation"
::notation::mc::MoveForm(ran)	"Reversible Algebraic Notation"
::notation::mc::MoveForm(smi)	"Smith Notation"
::notation::mc::MoveForm(edn)	"English Descriptive Notation"
::notation::mc::MoveForm(sdn)	"Spanish Descriptive Notation"
::notation::mc::MoveForm(cor)	"ICCF Numeric Notation (Correspondence)"
::notation::mc::MoveForm(tel)	"Alphabetic Notation (Telegraph)"

### figurine ###########################################################
::figurines::mc::Figurines	"Figurines"
::figurines::mc::Graphic	"Grafica"
::figurines::mc::User		"Utente" ;# "Definito dall'utente"

### save/replace #######################################################
::dialog::save::mc::SaveGame						"Salva Partita"
::dialog::save::mc::ReplaceGame					"Sostituisci Partita"
::dialog::save::mc::EditCharacteristics		"Modifica caratteristiche"
	
::dialog::save::mc::PressToSelect				"Premi da Ctrl-0 a Ctrl-9 (o tasto sinistro del mouse) per selezionare"
::dialog::save::mc::PressForWhole				"Premi da Alt-0 a Alt-9 (o tasto centrale del mouse) per l'intera selezione dati"
::dialog::save::mc::EditTags						"Modifica Tag"
::dialog::save::mc::RemoveThisTag				"Eliminare tag '%s'?"
::dialog::save::mc::TagAlreadyExists			"Il nome tag '%s' esiste già."
::dialog::save::mc::TagRemoved					"La tag '%s' (valore attuale: '%s') sarà eliminata."
::dialog::save::mc::TagNameIsReserved			"Nome tag '%s' è riservato."
::dialog::save::mc::Locked							"Chiuso"
::dialog::save::mc::OtherTag						"Altra tag"
::dialog::save::mc::NewTag							"Aggiungi tag"
::dialog::save::mc::RemoveTag						"Eliminare tag"
::dialog::save::mc::SetToGameDate				"Imposta a data partita"
::dialog::save::mc::SaveGameFailed				"Salvataggio partita fallito."
::dialog::save::mc::SaveGameFailedDetail		"Guarda il log per maggiori dettagli."
::dialog::save::mc::SavingGameLogInfo			"Salvataggio partita (%white - %black, %event) nel database '%base'"
::dialog::save::mc::CurrentBaseIsReadonly		"L'attuale database '%s' è per sola-lettura."
::dialog::save::mc::CurrentGameHasTrialMode	"L'attuale partita è in modalità di prova e non può essere salvata."
::dialog::save::mc::LeaveTrialModeHint			"Prima devi uscire dalla modalità di prova, usa la scorciatoia %s."
::dialog::save::mc::OpenPlayerDictionary		"Apri Dizionario Giocatore"
::dialog::save::mc::TagName						"Tag '%s'"
::dialog::save::mc::InSection						"in section '%s'" ;# NEW
::dialog::save::mc::StringTooLong				"La stringa <small><fixed>%value%</fixed></small> è troppo lunga e sarà troncata a <small><fixed>%trunc%</fixed></small>"

::dialog::save::mc::LocalName						"&Nome locale"
::dialog::save::mc::EnglishName					"Nome &Inglese"
::dialog::save::mc::ShowRatingType				"Mostra &punteggio"
::dialog::save::mc::EcoCode						"&Codice ECO"
::dialog::save::mc::Matches						"&Match"
::dialog::save::mc::Tags							"&Tags"

::dialog::save::mc::Section(game)				"Dati Partita"
::dialog::save::mc::Section(event)				"Evento"
::dialog::save::mc::Section(white)				"Bianco"
::dialog::save::mc::Section(black)				"Nero"
::dialog::save::mc::Section(tags)				"Concordanze Tag / Estranee"

::dialog::save::mc::Label(name)					"Nome"
::dialog::save::mc::Label(fideID)				"Fide-ID"
::dialog::save::mc::Label(value)					"Valore"
::dialog::save::mc::Label(title)					"Titolo"
::dialog::save::mc::Label(rating)				"Elo"
::dialog::save::mc::Label(federation)			"Federazione"
::dialog::save::mc::Label(country)				"Nazione"
::dialog::save::mc::Label(eventType)			"Tipo"
::dialog::save::mc::Label(sex)					"Sesso/Tipe"
::dialog::save::mc::Label(date)					"Data"
::dialog::save::mc::Label(eventDate)			"Data Evento"
::dialog::save::mc::Label(round)					"Turno"
::dialog::save::mc::Label(result)				"Risultato"
::dialog::save::mc::Label(termination)			"Terminazione"
::dialog::save::mc::Label(annotator)			"Commentatore"
::dialog::save::mc::Label(site)					"Luogo"
::dialog::save::mc::Label(eventMode)			"Modalità"
::dialog::save::mc::Label(timeMode)				"Cadenza"
::dialog::save::mc::Label(frequency)			"Frequenza"
::dialog::save::mc::Label(score)					"Rating" ;#"Second rating"

::dialog::save::mc::GameBase						"Base Partite"
::dialog::save::mc::PlayerBase					"Base Giocatori"
::dialog::save::mc::EventBase						"Base Eventi"
::dialog::save::mc::SiteBase						"Base Siti"
::dialog::save::mc::AnnotatorBase				"Base Commentatori"
::dialog::save::mc::History						"Storia"

::dialog::save::mc::InvalidEntry					"'%s' non è un valore valido."
::dialog::save::mc::InvalidRoundEntry			"'%s' non è un valido valore di turno."
::dialog::save::mc::InvalidRoundEntryDetail	"Valori di turno validi sono '4' o '6.1'. Lo zero non è permesso."
::dialog::save::mc::RoundIsTooHigh				"I turni devono essere meno di 256."
::dialog::save::mc::SubroundIsTooHigh			"I sotto-turni devono essere meno di 256."
::dialog::save::mc::ImplausibleDate				"La data della partita '%date' è precedente a quella dell'evento '%eventdate'."
::dialog::save::mc::InvalidTagName				"Nome tag non valido '%s' (errore di sintassi)."
::dialog::save::mc::Field							"Campo '%s': "
::dialog::save::mc::ExtraTag						"Tag Extra '%s': "
::dialog::save::mc::InvalidNetworkAddress		"Indirizzo network non valido '%s'."
::dialog::save::mc::InvalidCountryCode			"Codice nazione non valido '%s'."
::dialog::save::mc::InvalidEventRounds			"Numero di turni per l'evento non valido '%s' (serve un intero positivo)."
::dialog::save::mc::InvalidPlyCount				"Numero mosse non valido '%s' (serve un intero positivo)."
::dialog::save::mc::IncorrectPlyCount			"Numero mosse scorretto '%s' (il numero corretto è %s)."
::dialog::save::mc::InvalidTimeControl			"Cadenza inserita in '%s' non valida."
::dialog::save::mc::InvalidDate					"Data non valida '%s'."
::dialog::save::mc::InvalidYear					"Anno non valido '%s'."
::dialog::save::mc::InvalidMonth					"Mese non valido '%s'."
::dialog::save::mc::InvalidDay					"Giorno non valido '%s'."
::dialog::save::mc::MissingYear					"Manca l'anno."
::dialog::save::mc::MissingMonth					"Manca il mese."
::dialog::save::mc::InvalidEventDate			"Non posso accettare la data inserita: La differenza tra l'anno della partita e l'anno dell'evento dovrebbe essere meno di 4 (restrizione del formato database di Scid)."
::dialog::save::mc::TagIsEmpty					"Tag is empty (will be discarded)."

### gamehistory ########################################################
::game::history::mc::GameHistory	"Storia della Partita"

### game ###############################################################
::game::mc::CloseDatabase					"Chiudi Database"
::game::mc::CloseAllGames					"Chiudere tutte le partite aperte del database '%s'?"
::game::mc::SomeGamesAreModified			"Alcune partite nel database '%s' sono state modificate. Chiudo comunque?"
::game::mc::AllSlotsOccupied				"Tutti i posti per le partite sono occupati."
::game::mc::ReleaseOneGame					"Per favore chiudi una partita prima di caricarne un'altra."
::game::mc::GameAlreadyOpen				"La partita è già aperta ma modificata. Annulla modifiche a questa partita?"
::game::mc::GameAlreadyOpenDetail		"'%s' aprirà una nuova partita."
::game::mc::GameHasChanged					"La partita %s è stata modificata."
::game::mc::GameHasChangedDetail			"Probabilmente questa non è la partita giusta in virtù di cambiamenti nel database"
::game::mc::CorruptedHeader				"Intestazione corrotta nel file di ripristino '%s'." ;#di ripristino? da ripristinare?
::game::mc::RenamedFile						"File rinominato in '%s.bak'."
::game::mc::CannotOpen						"Impossibile aprire file di ripristino '%s'."
::game::mc::GameRestored					"Una partita dall'ultima sessione ripristinata."
::game::mc::GamesRestored					"%s partite dall'ultima sessione ripristinate."
::game::mc::OldGameRestored				"Una partita ripristinata."
::game::mc::OldGamesRestored				"%s partite ripristinate."
::game::mc::ErrorInRecoveryFile			"Errore nel file di ripristino '%s'"
::game::mc::Recovery							"Ripristino"
::game::mc::UnsavedGames					"Ci sono modifiche non salvate."
::game::mc::DiscardChanges					"'%s' annullerà ogni modifica."
::game::mc::ShouldRestoreGame				"Vuoi ripristinare questa partita nella prossima sessione?"
::game::mc::ShouldRestoreGames			"Vuoi ripristinare queste partite nella prossima sessione?"
::game::mc::NewGame							"Nuova partita"
::game::mc::NewGames							"Nuove partite"
::game::mc::Created							"creata" ;#creato?
::game::mc::ClearHistory					"Pulisci Storia"
::game::mc::RemoveSelectedGame			"Rimuovi partita selezionata dalla storia"
::game::mc::GameDataCorrupted				"Dati partita corrotti."
::game::mc::GameDecodingFailed			"La decodifica di questa partita non è stata possibile."
::game::mc::GameDecodingChanged			"Il database è aperto con il set caratteri '%base%', ma questa partita sembra codificata con il set caratteri '%game%', quindi la partita è caricata con il set caratteri identificato."
::game::mc::GameDecodingChangedDetail	"Probabilmente hai aperto il database con il set caratteri sbagliato. Nota che l'identificazione automatica del set caratteri è limitata."
::game::mc::VariantHasChanged				"La partita non può essere aperta perché la variante del database è cambiata ed è differente dalla variante della partita."
::game::mc::RemoveGameFromHistory		"Rimuovi partita dalla storia?"
::game::mc::GameNumberDoesNotExist		"La partita %number non esiste in '%base'."
::game::mc::ReallyReplaceGame				"Sembra che la partita #%s nell'editor non è la partita originariamente aperta data una modifica avvenuta nel database, è possibile che un'altra partita verrà persa. Vuoi davvero rimpiazzare i dati?"
::game::mc::ReallyReplaceGameDetail		"Si raccomanda di guardare la partita #%s prima di continuare con questa operazione."
::game::mc::ReopenLockedGames				"Ri-apri partite bloccate dalla sessione precedente?"
::game::mc::OpenAssociatedDatabases		"Apri tutti i database associati?"
::game::mc::OverwriteCurrentGame			"Overwrite current game?" ;# NEW
::game::mc::OverwriteCurrentGameDetail	"A new game will be opened if answered with '%s'." ;# NEW

### searchentry ########################################################
::searchentry::mc::Erase					"Erase" ;# NEW
::searchentry::mc::FindNext				"Find Next" ;# NEW
::searchentry::mc::InteractiveSearch	"Interactive Search" ;# NEW

### languagebox ########################################################
::languagebox::mc::AllLanguages	"Tutte le lingue"
::languagebox::mc::None				"Nessuno"

### ecobox #############################################################
::ecobox::mc::OpenEcoDialog "Open ECO dialog" ;# NEW

### datebox ############################################################
::datebox::mc::Today		"Oggi"
::datebox::mc::Calendar	"Calendario..."
::datebox::mc::Year		"Anno"
::datebox::mc::Month		"Mese"
::datebox::mc::Day		"Giorno"

::datebox::mc::Hint(Space)	"Clear" ;# NEW
::datebox::mc::Hint(?)		"Open calendar" ;# NEW
::datebox::mc::Hint(!)		"Set to game date" ;# NEW
::datebox::mc::Hint(=)		"Skip entering" ;# NEW

### genderbox ##########################################################
::genderbox::mc::Gender(m) "Maschio"
::genderbox::mc::Gender(f) "Femmina"
::genderbox::mc::Gender(c) "Computer"
::genderbox::mc::Gender(?) "Unspecified" ;# NEW

### terminationbox #####################################################
::terminationbox::mc::Normal								"Normale"
::terminationbox::mc::Unplayed							"Non Giocata"
::terminationbox::mc::Abandoned							"Abbandonata"
::terminationbox::mc::Adjudication						"Aggiudicata"
::terminationbox::mc::Disconnection						"Disconnessione"
::terminationbox::mc::Emergency							"Emergenza"
::terminationbox::mc::RulesInfraction					"Infrazione regole"
::terminationbox::mc::TimeForfeit						"Tempo scaduto"
::terminationbox::mc::Unterminated						"Indeterminato"

::terminationbox::mc::State(Checkmate)					"%s è scacco matto"
::terminationbox::mc::State(Stalemate)					"%s è stallo"
::terminationbox::mc::State(ThreeChecks)				"%s ha preso tre scacchi"
::terminationbox::mc::State(Losing)						"%s vince in quanto ha perso tutto il materiale"

::terminationbox::mc::Result(1-0)						"Il nero ha abbandonato"
::terminationbox::mc::Result(0-1)						"Il bianco ha abbandonato"
::terminationbox::mc::Result(0-0)						"Dichiarata persa per entrambi i giocatori"
::terminationbox::mc::Result(1/2-1/2)					"Patta per accordo"

::terminationbox::mc::Reason(Unplayed)					"La partita non è stata giocata"
::terminationbox::mc::Reason(ByForfeit)				"Opponent did not show up" ;# NEW
::terminationbox::mc::Reason(Abandoned)				"La partita è stata abbandonata"
::terminationbox::mc::Reason(Adjudication)			"Aggiundicata"
::terminationbox::mc::Reason(Disconnection)			"Disconnessione"
::terminationbox::mc::Reason(Emergency)				"Abbandonata per via di un'emergenza"
::terminationbox::mc::Reason(RulesInfraction)		"Decisa in virtù di infrazioni alle regole"
::terminationbox::mc::Reason(TimeForfeit)				"%s perde per tempo"
::terminationbox::mc::Reason(TimeForfeit,both)		"Entrambi i giocatori perdono per tempo"
::terminationbox::mc::Reason(TimeForfeit,remis)		"%causer ha finito il tempo e %opponent non ha materiale per vincere"
::terminationbox::mc::Reason(NoOpponent)				"Point given for game with no opponent" ;# NEW
::terminationbox::mc::Reason(Unterminated)			"No finalizado" ;# NEW
::terminationbox::mc::Reason(Unterminated)			"Non terminata"

::terminationbox::mc::Termination(checkmate)			"%s è scacco matto"
::terminationbox::mc::Termination(stalemate)			"%s è stallo"
::terminationbox::mc::Termination(three-checks)		"%s ha preso tre scacchi"
::terminationbox::mc::Termination(material)			"%s vince in quanto ha perso tutto il materiale"
::terminationbox::mc::Termination(equal-material)	"Partita patta per stallo (materiale equo)"
::terminationbox::mc::Termination(less-material)	"%s vince in quanto ha meno materiale (stallo)"
::terminationbox::mc::Termination(bishops)			"Partita patta per stallo (alfieri di colore opposto)"
::terminationbox::mc::Termination(fifty)				"Partita patta per la regola delle 50 mosse"
::terminationbox::mc::Termination(threefold)			"Partita patta per triplice ripetizione"
::terminationbox::mc::Termination(fivefold)			"Game drawn by fivefold repetition" ;# NEW
::terminationbox::mc::Termination(nomating)			"Partita patta per insufficienza di materiale"
::terminationbox::mc::Termination(nocheck)			"Nessun giocatore può dare scacco"

### eventmodebox #######################################################
::eventmodebox::mc::OTB				"A tavolino"
::eventmodebox::mc::PM				"Corrispondenza"
::eventmodebox::mc::EM				"E-mail"
::eventmodebox::mc::ICS				"Internet Chess Server"
::eventmodebox::mc::TC				"Telecomunicazione"
::eventmodebox::mc::Analysis		"Analisi"
::eventmodebox::mc::Composition	"Composizione"

### eventtypebox #######################################################
::eventtypebox::mc::Type(casual)	"Partita singola"
::eventtypebox::mc::Type(match)	"Match"
::eventtypebox::mc::Type(tourn)	"Round Robin"
::eventtypebox::mc::Type(swiss)	"Torneo con Sistema Svizzero"
::eventtypebox::mc::Type(team)	"Torneo a squadre"
::eventtypebox::mc::Type(k.o.)	"Torneo Knockout"
::eventtypebox::mc::Type(simul)	"Torneo in simultanea"
::eventtypebox::mc::Type(schev)	"Torneo con Sistema Scheveningen"

### timemodebox ########################################################
::timemodebox::mc::Mode(normal)	"Normale"
::timemodebox::mc::Mode(rapid)	"Rapid"
::timemodebox::mc::Mode(blitz)	"Blitz"
::timemodebox::mc::Mode(bullet)	"Bullet"
::timemodebox::mc::Mode(corr)		"Corrispondenza"

### help ###############################################################
::help::mc::Contents					"&Contenuti"
::help::mc::Index						"&Indice"
::help::mc::CQL						"C&QL"
::help::mc::Search					"Ce&rca"

::help::mc::Help						"Aiuto"
::help::mc::MatchEntireWord		"Corrispondi intera parola"
::help::mc::MatchCase				"Corrispondi capitalizzazione"
::help::mc::TitleOnly				"Cerca solo nei titoli"
::help::mc::CurrentPageOnly		"Cerca solo nella pagina corrente"
::help::mc::GoBack					"Vai indietro una pagina"
::help::mc::GoForward				"Vai avanti una pagina"
::help::mc::GotoHome					"Vai all'inizio della pagina"
::help::mc::GotoEnd					"Vai alla fine della pagina"
::help::mc::GotoPage					"Vai alla pagina '%s'"
::help::mc::NextTopic				"Vai all'argomento successivo"
::help::mc::PrevTopic				"Vai all'argomento precedente"
::help::mc::ExpandAllItems			"Espandi tutti gli oggetti"
::help::mc::CollapseAllItems		"Comprimi tutti gli oggetti"
::help::mc::SelectLanguage			"Selezione Lingua"
::help::mc::NoHelpAvailable		"Nessun file di aiuto disponibile per la lingua italiana.\nPer favore scelti una lingua alternativa\nper la finestra di aiuto."
::help::mc::NoHelpAvailableAtAll	"Nessun file di aiuto disponibile per questo argomento."
::help::mc::KeepLanguage			"Mantenere la lingua %s per le prossime sessioni?"
::help::mc::ParserError				"Errore nel parsing del file %s."
::help::mc::NoMatch					"Nessuna corrispondenza trovata"
::help::mc::MaxmimumExceeded		"Il numero massimo di corrispondenze è ecceduto in alcune pagine."
::help::mc::OnlyFirstMatches		"Solo le prime %s corrispondenze per pagina saranno mostrate."
::help::mc::HideIndex				"Nascondi Indice"
::help::mc::ShowIndex				"Mostra Indice"
::help::mc::All						"Tutti"

::help::mc::FileNotFound			"File non trovato."
::help::mc::CantFindFile			"Impossibile trovare file a %s."
::help::mc::IncompleteHelpFiles	"I file di aiuto sono ancora incompleti. Ci scusiamo."
::help::mc::ProbablyTheHelp		"Probabilmente la pagina di aiuto in una lingua differente può essere una valida alternativa."
::help::mc::PageNotAvailable		"Questa pagina non è disponibile"

::help::mc::TextAlignment			"Text alignment" ;# NEW
::help::mc::FullJustification		"Full justification" ;# NEW
::help::mc::LeftJustification		"Left justification" ;# NEW

### crosstable #########################################################
::crosstable::mc::TournamentTable			"Tabella Torneo"
::crosstable::mc::AverageRating				"Elo medio"
::crosstable::mc::Category						"Categoria"
::crosstable::mc::Games							"partite"
::crosstable::mc::Game							"partita"

::crosstable::mc::ScoringSystem				"Sistema di Punteggio"
::crosstable::mc::Tiebreak						"Tie-Break"
::crosstable::mc::Settings						"Opzioni"
::crosstable::mc::RevertToStart				"Torna ai valori iniziali"
::crosstable::mc::UpdateDisplay				"Aggiorna visualizzazione"
::crosstable::mc::SaveAsHTML					"Salva come file HTML"

::crosstable::mc::Traditional					"Tradizionale"
::crosstable::mc::Bilbao						"Bilbao"

::crosstable::mc::None							"Nessuna"
::crosstable::mc::Buchholz						"Buchholz"
::crosstable::mc::MedianBuchholz				"Median-Buchholz"
::crosstable::mc::ModifiedMedianBuchholz	"Mod. Median-Buchholz"
::crosstable::mc::RefinedBuchholz			"Buchholz Migliorato"
::crosstable::mc::SonnebornBerger			"Sonneborn-Berger"
::crosstable::mc::Progressive					"Punteggio progressivo"
::crosstable::mc::KoyaSystem					"Sistema Koya"
::crosstable::mc::GamesWon						"Numero di partite vinte"
::crosstable::mc::GamesWonWithBlack			"Partite vinte dal Nero"
::crosstable::mc::ParticularResult			"Risultato Particolare"
::crosstable::mc::TraditionalScoring		"Punteggio Tradizionale"

::crosstable::mc::Crosstable					"Tabellone"
::crosstable::mc::Scheveningen				"Scheveningen"
::crosstable::mc::Swiss							"Sistema svizzero"
::crosstable::mc::Match							"Match"
::crosstable::mc::Knockout						"Knockout"
::crosstable::mc::RankingList					"Lista ELO"
::crosstable::mc::Simultan						"Simultaneous" ;# NEW

::crosstable::mc::Order							"Ordine"
::crosstable::mc::Type							"Tipo Tabella"
::crosstable::mc::Score							"Punteggio"
::crosstable::mc::Alphabetical				"Alfabetico"
::crosstable::mc::Rating						"Elo"
::crosstable::mc::Federation					"Federazione"

::crosstable::mc::Debugging					"Debugging"
::crosstable::mc::Display						"Display"
::crosstable::mc::Style							"Stile"
::crosstable::mc::Spacing						"Spaziatura"
::crosstable::mc::Padding						"Padding"
::crosstable::mc::ShowLog						"Mostra Log"
::crosstable::mc::ShowHtml						"Mostra HTML"
::crosstable::mc::ShowRating					"Elo"
::crosstable::mc::ShowPerformance			"Performance"
::crosstable::mc::ShowWinDrawLoss			"Vinte/Patte/Perse"
::crosstable::mc::ShowTiebreak				"Tiebreak"
::crosstable::mc::ShowOpponent				"Avversario (come Tooltip)" ;# ??
::crosstable::mc::KnockoutStyle				"Stile Tabella Knockout"
::crosstable::mc::Pyramid						"Piramide"
::crosstable::mc::Triangle						"Triangolo"

::crosstable::mc::CrosstableLimit			"Il limite del tabellone di %d giocatori sarà superato."
::crosstable::mc::CrosstableLimitDetail	"'%s' sta scegliendo un altro stile tabella."
::crosstable::mc::CannotOverwriteFile		"Impossibile sovrascrivere il file '%s': permesso negato."
::crosstable::mc::CannotCreateFile			"Impossibile creare il file '%s': permesso negato."

### info ###############################################################
::info::mc::InfoTitle				"About %s"
::info::mc::Info						"Info"
::info::mc::About						"About"
::info::mc::Contributions			"Contributi"
::info::mc::License					"Licenza"
::info::mc::Localization			"Localizzazione"
::info::mc::Testing					"Testing"
::info::mc::References				"Riferimenti"
::info::mc::System					"Sistema"
::info::mc::FontDesign				"Design del font di scacchi"
::info::mc::TruetypeFonts			"Truetype fonts" ;# NEW
::info::mc::ChessPieceDesign		"Design dei pezzi"
::info::mc::BoardThemeDesign		"Design della scacchiera"
::info::mc::FlagsDesign				"Design identificatori miniaturizzati"
::info::mc::IconDesign				"Design icone"
::info::mc::Development				"Sviluppo"
::info::mc::DevelopmentOfUnCBV	"Development of unzipping CBV archives" ;# NEW
::info::mc::Programming				"Programmazione"
::info::mc::Head						"Intestazione"
::info::mc::AllOthers				"all others" ;# NEW
::info::mc::TheMissingOnes			"the missing ones" ;# NEW

::info::mc::Version					"Versione"
::info::mc::Distributed				"Questo programma è distribuito secondo i termini della GNU General Public License."
::info::mc::Inspired					"Scidb è ispirato a Scid 3.6.1, copyrighted \u00A9 1999-2003 by Shane Hudson."
::info::mc::SpecialThanks			"Ringraziamenti speciali a %s per il suo incredibile lavoro. I suoi sforzi sono alla base di questa applicazione."

### comment ############################################################
::comment::mc::CommentBeforeMove		"Commento prima della mossa"
::comment::mc::CommentAfterMove		"Commento dopo mossa"
::comment::mc::PrecedingComment		"Commento precedente"
::comment::mc::TrailingComment		"Commento successivo"
::comment::mc::Language					"Lingua"
::comment::mc::AddLanguage				"Aggiungi lingua..."
::comment::mc::SwitchLanguage			"Cambia lingua"
::comment::mc::FormatText				"Formatta testo"
::comment::mc::CopyText					"Copia testo in" ;#Copy text to
::comment::mc::OverwriteContent		"Sovrascrivere contenuto esistente?"
::comment::mc::AppendContent			"Se \"no\" il testo sarà aggiunto."
::comment::mc::DisplayEmoticons		"Mostra Emoticons"
::comment::mc::ReallySwitch			"Really switch display mode?" ;# NEW
::comment::mc::LosingChanges			"Switching the display mode will loose the history, this means you cannot undo the last edit operations." ;# NEW

::comment::mc::LanguageSelection		"Selezione lingua"
::comment::mc::Formatting				"Formattazione"
::comment::mc::InsertLink				"Insert link" ;# NEW

::comment::mc::Bold						"Grassetto"
::comment::mc::Italic					"Corsivo"
::comment::mc::Underline				"Sottolinea"

::comment::mc::InsertSymbol			"&Inserisci Simbolo..."
::comment::mc::InsertEmoticon			"Inserisci &Emoticon..."
::comment::mc::MiscellaneousSymbols	"Simboli vari"
::comment::mc::Figurine					"Figurine"

### annotation #########################################################
::annotation::mc::AnnotationEditor					"Annotazione"
::annotation::mc::TooManyNags							"Troppe annotazioni (l'ultima è stato ignorato)."
::annotation::mc::TooManyNagsDetail					"Massimo %d annotazioni per semimossa permessi."

::annotation::mc::PrefixedCommentaries				"Commenti prima della mossa"
::annotation::mc::MoveAssesments						"Giudizio sulla mossa"
::annotation::mc::PositionalAssessments			"Giudizi posizionali"
::annotation::mc::TimePressureCommentaries		"Commenti Zeitnot"
::annotation::mc::AdditionalCommentaries			"Commenti aggiuntivi"
::annotation::mc::ChessBaseCommentaries			"Commenti ChessBase"

### marks ##############################################################
::marks::mc::MarksPalette			"Tavolozza Simboli"

### move ###############################################################
::move::mc::Action(replace)		"Sostituisci mossa"
::move::mc::Action(variation)		"Aggiungi nuova variante"
::move::mc::Action(mainline)		"Nuova linea principale"
::move::mc::Action(trial)			"Prova variante"
::move::mc::Action(exchange)		"Cambia mossa"
::move::mc::Action(append)			"Aggiungi mossa in cosa"
::move::mc::Action(load)			"Carica la prima partita con questa continuazione"

::move::mc::Accel(trial)			"T"
::move::mc::Accel(replace)			"R"
::move::mc::Accel(variation)		"V"
::move::mc::Accel(append)			"A"
::move::mc::Accel(load)				"L"

::move::mc::GameWillBeTruncated	"La partita sarà interrotta. Continuare con '%s'?"

### log ################################################################
::log::mc::LogTitle		"Log"
::log::mc::Warning		"Avvertenza"
::log::mc::Error			"Errore"
::log::mc::Information	"Info"

### titlebox ############################################################
::titlebox::mc::None				"Nessun titolo"
::titlebox::mc::Title(GM)		"Grande Maestro (FIDE)"
::titlebox::mc::Title(IM)		"Maestro Internazionale (FIDE)"
::titlebox::mc::Title(FM)		"Maestro Fide (FIDE)"
::titlebox::mc::Title(CM)		"Candidato Maestro (FIDE)"
::titlebox::mc::Title(WGM)		"Grande Mestro Femminile (FIDE)"
::titlebox::mc::Title(WIM)		"Maestro Internazionale Femminile (FIDE)"
::titlebox::mc::Title(WFM)		"Maestro Fide Femminile (FIDE)"
::titlebox::mc::Title(WCM)		"Candidato Maestro Femminile (FIDE)"
::titlebox::mc::Title(HGM)		"Grande Maestro ad honorem (FIDE)"
::titlebox::mc::Title(NM)		"Maestro Nazionale (USCF)"
::titlebox::mc::Title(SM)		"Maestro Senior (USCF)"
::titlebox::mc::Title(LM)		"Maestro a vita (USCF)"
::titlebox::mc::Title(CGM)		"Grande Maestro per corrispondenza (ICCF)"
::titlebox::mc::Title(CIM)		"Maestro Internazionale per corrispondenza (ICCF)"
::titlebox::mc::Title(CLGM)	"Grande Maestro femminile per corrispondenza (ICCF)"
::titlebox::mc::Title(CLIM)	"Maestro Internazionale femminile per corrispondenza (ICCF)"
::titlebox::mc::Title(CSIM)	"Maestro Internazionale Senior per corrispondenza (ICCF)"

### messagebox #########################################################
::dialog::mc::Ok				"&OK"
::dialog::mc::Cancel			"&Annulla"
::dialog::mc::Yes				"&Sì"
::dialog::mc::No				"&No"
::dialog::mc::Retry			"&Riprova"
::dialog::mc::Abort			"&Interrompi"
::dialog::mc::Ignore			"I&gnora"
::dialog::mc::Continue		"Con&tinua"

::dialog::mc::Error			"Errore"
::dialog::mc::Warning		"Avvertenza"
::dialog::mc::Information	"Informazioni"
::dialog::mc::Question		"Conferma"

::dialog::mc::DontAskAgain	"Non chiedere più"

### web ################################################################
::web::mc::CannotFindBrowser			"Non ho trovato un browser compatibile."
::web::mc::CannotFindBrowserDetail	"Imposta la variabile d'ambiente BROWSER al tuo browser desiderato."

### colormenu ##########################################################
::colormenu::mc::BaseColor			"Base Colori"
::colormenu::mc::UserColor			"Colori utente"
::colormenu::mc::UsedColor			"Colori usati"
::colormenu::mc::RecentColor		"Colori recenti"
::colormenu::mc::Texture			"Texture"
::colormenu::mc::OpenColorDialog	"Apri finestra colori"
::colormenu::mc::EraseColor		"Elimina colore"
::colormenu::mc::Close				"Chiudi"

### table ##############################################################
::table::mc::Ok							"&Ok"
::table::mc::Cancel						"&Annulla"
::table::mc::Column						"Colonna"
::table::mc::Table						"Tabella"
::table::mc::Configure					"Configura"
::table::mc::Hide							"Nascondi"
::table::mc::ShowColumn					"Mostra colonna"
::table::mc::Foreground					"Primo Piano"
::table::mc::Background					"Sfondo"
::table::mc::DisabledForeground		"Primo Piano eliminato"
::table::mc::SelectionForeground		"Seleziona Primo Piano"
::table::mc::SelectionBackground		"Selezione Sfondo"
::table::mc::HighlightColor			"Evidenzia Sfondo"
::table::mc::Stripes						"Strisce"
::table::mc::MinWidth					"Larghezza minima"
::table::mc::MaxWidth				   "Larghezza massima"
::table::mc::Separator					"Separatore"
::table::mc::AutoStretchColumn		"Espandi colonne automaticamente"
::table::mc::FillColumn					"- Riempi colonna -"
::table::mc::Preview						"Anteprima"
::table::mc::OptimizeColumn			"Ottimizza larghezza colonna"
::table::mc::OptimizeColumns			"Ottimizza tutte le colonne"
::table::mc::FitColumnWidth			"Regola larghezza colonna"
::table::mc::FitColumns					"Regola tutte le colonne"
::table::mc::ExpandColumn				"Espandi larghezza colonna"
::table::mc::ShrinkColumn				"Shrink column width" ;# NEW
::table::mc::SqueezeColumns			"Stringi tutte le colonne"
::table::mc::AccelFitColumns			"Ctrl+,"
::table::mc::AccelOptimizeColumns	"Ctrl+."
::table::mc::AccelSqueezeColumns		"Ctrl+#"

### fileselectionbox ###################################################
::dialog::fsbox::mc::ScidbDatabase			"Database Scidb"
::dialog::fsbox::mc::ScidDatabase			"Database Scidb"
::dialog::fsbox::mc::ChessBaseDatabase		"Database ChessBase"
::dialog::fsbox::mc::PortableGameFile		"File Portatile di partita" ;# Notazione partita a scacchi PGN
::dialog::fsbox::mc::PortableGameFileCompressed "File Portatile di partita (compresso con gzip)"
::dialog::fsbox::mc::BughousePortableGameFile "File Portatile di Partita Mangia-Passa" ;# Notazione partita a Mangia-Passa BPGN
::dialog::fsbox::mc::BughousePortableGameFileCompressed "File Portatile di Partita Mangia-Passa (compresso con gzip)"
::dialog::fsbox::mc::ZipArchive				"Archivio ZIP"
::dialog::fsbox::mc::ScidbArchive			"Arvchivio Scidb"
::dialog::fsbox::mc::PortableDocumentFile	"Documento Portatile"
::dialog::fsbox::mc::HypertextFile			"File Ipertestuale"
::dialog::fsbox::mc::TypesettingFile		"File di Typesetting"
::dialog::fsbox::mc::ImageFile				"File di immagine"
::dialog::fsbox::mc::TextFile					"File di testo"
::dialog::fsbox::mc::BinaryFile				"File binario"
::dialog::fsbox::mc::ShellScript				"Script Shell"
::dialog::fsbox::mc::Executable				"Eseguibile"

::dialog::fsbox::mc::LinkTo					"Collegamento a %s"
::dialog::fsbox::mc::LinkTarget				"Obiettivo collegamento"
::dialog::fsbox::mc::Directory				"Cartella"

::dialog::fsbox::mc::Title(open)				"Seleziona File"
::dialog::fsbox::mc::Title(save)				"Salva File"
::dialog::fsbox::mc::Title(dir)				"Scegli Directory"

::dialog::fsbox::mc::Content					"Contenuto"
::dialog::fsbox::mc::Open						"Apri"
::dialog::fsbox::mc::OriginalPath			"Percorso Originale"
::dialog::fsbox::mc::DateOfDeletion			"Data di Eliminazione"
::dialog::fsbox::mc::Readonly					"Readonly" ;# NEW

::dialog::fsbox::mc::FileType(exe)			"Eseguibili"
::dialog::fsbox::mc::FileType(txt)			"Files di testo"
::dialog::fsbox::mc::FileType(bin)			"Files binari"
::dialog::fsbox::mc::FileType(log)			"Files di log"
::dialog::fsbox::mc::FileType(html)			"Files HTML"

### choosecolor ########################################################
::dialog::choosecolor::mc::Ok					"&OK"
::dialog::choosecolor::mc::Cancel			"&Cancella"

::dialog::choosecolor::mc::BaseColors		"Base Colori"
::dialog::choosecolor::mc::UserColors		"Colori utente"
::dialog::choosecolor::mc::RecentColors	"Colori recenti"
::dialog::choosecolor::mc::Old				"Vecchio"
::dialog::choosecolor::mc::Current			"Attuale"
::dialog::choosecolor::mc::HexCode			"Codice Esadecimale"
::dialog::choosecolor::mc::ColorSelection	"Selezione Colore"
::dialog::choosecolor::mc::Red				"Rosso"
::dialog::choosecolor::mc::Green				"Verde"
::dialog::choosecolor::mc::Blue				"Blu"
::dialog::choosecolor::mc::Hue				"Tonalità"
::dialog::choosecolor::mc::Saturation		"Saturazione"
::dialog::choosecolor::mc::Value				"Valore"
::dialog::choosecolor::mc::Enter				"Inserisci"
::dialog::choosecolor::mc::AddColor			"Aggiunti colore attuali ai colori utente"

### choosefont #########################################################
::dialog::choosefont::mc::Apply				"&Applica"
::dialog::choosefont::mc::Cancel				"A&nnulla"
::dialog::choosefont::mc::Continue			"Con&tinua"
::dialog::choosefont::mc::FixedOnly			"Solo font &Monospaced"
::dialog::choosefont::mc::Family				"Fam&iglia"
::dialog::choosefont::mc::Font				"&Font"
::dialog::choosefont::mc::Ok					"&OK"
::dialog::choosefont::mc::Reset				"&Reset"
::dialog::choosefont::mc::Size				"&Grandezza"
::dialog::choosefont::mc::Strikeout			"Can&cellato"
::dialog::choosefont::mc::Style				"&Stile"
::dialog::choosefont::mc::Underline			"S&ottolinea"
::dialog::choosefont::mc::Color				"Co&lore"

::dialog::choosefont::mc::Regular			"Regolare"
::dialog::choosefont::mc::Bold				"Grassetto"
::dialog::choosefont::mc::Italic				"Corsivo"
{::dialog::choosefont::mc::Bold Italic}	"Grassetto Corsivo"

::dialog::choosefont::mc::Effects			"Effetti"
::dialog::choosefont::mc::Filter				"Filtro"
::dialog::choosefont::mc::Sample				"Campione"
::dialog::choosefont::mc::SearchTitle		"Ricerca per font monospaced"
::dialog::choosefont::mc::FontSelection	"Seleziona Font"
::dialog::choosefont::mc::Wait				"Aspetta"

### choosedir ##########################################################
::choosedir::mc::ShowPredecessor	"Mostra Precedente"
::choosedir::mc::ShowTail			"Mostra Coda"
::choosedir::mc::Folder				"Cartella"

### fsbox ##############################################################
::fsbox::mc::Name								"Nome"
::fsbox::mc::Size								"Grandezza"
::fsbox::mc::Modified						"Modificato"

::fsbox::mc::Forward							"Avanti a '%s'"
::fsbox::mc::Backward						"Indietro a '%s'"
::fsbox::mc::Delete							"Elimina"
::fsbox::mc::MoveToTrash					"Sposta nel Cestino"
::fsbox::mc::Restore							"Ripristina"
::fsbox::mc::Duplicate						"Duplica"
::fsbox::mc::CopyOf							"Copia di %s"
::fsbox::mc::NewFolder						"Nuova Cartella"
::fsbox::mc::Layout							"Layout"
::fsbox::mc::ListLayout						"Layout della Lista"
::fsbox::mc::DetailedLayout				"Layout Dettagliato"
::fsbox::mc::ShowHiddenDirs				"Mostra carte&lle nascoste"
::fsbox::mc::ShowHiddenFiles				"Mostra &file e cartelle nascoste"
::fsbox::mc::AppendToExisitingFile		"&Aggiungi partite a un file esistente"
::fsbox::mc::Cancel							"&Cancella"
::fsbox::mc::Save								"&Salva"
::fsbox::mc::Open								"A&pri"
::fsbox::mc::Overwrite						"&Sovrascrivi"
::fsbox::mc::Rename							"&Rinomina"
::fsbox::mc::Move								"Muovi"

::fsbox::mc::AddBookmark					"Aggiungi Segnalibro '%s'"
::fsbox::mc::RemoveBookmark				"Rimuovi il segnalibro '%s'"
::fsbox::mc::RenameBookmark				"Rinomina Segnalibro '%s'"

::fsbox::mc::Filename						"Nom&e file:"
::fsbox::mc::Filenames						"Nom&i file:"
::fsbox::mc::Directory						"&Cartella:"
::fsbox::mc::FilesType						"File del &tipo:"
::fsbox::mc::FileEncoding					"&Codifica File:"

::fsbox::mc::Favorites						"Preferiti"
::fsbox::mc::LastVisited					"Ultime Visite"
::fsbox::mc::FileSystem						"File System"
::fsbox::mc::Desktop							"Desktop"
::fsbox::mc::Trash							"Cestino"
::fsbox::mc::Download						"Download"
::fsbox::mc::Home								"Home"

::fsbox::mc::SelectEncoding				"Selezionare la codifica del database"
::fsbox::mc::SelectWhichType				"Selezione che tipo di file visualizzare"
::fsbox::mc::TimeFormat						"%d/%m/%Y %H:%M"

::fsbox::mc::CannotChangeDir				"Non posso cambiare la cartella a '%s'.\nPermesso negato."
::fsbox::mc::DirectoryRemoved				"Non posso passare alla cartella '%s'.\nLa cartella è rimossa."
::fsbox::mc::DeleteFailed					"Rimozione di '%s' fallita."
::fsbox::mc::RestoreFailed					"Ripristino di '%s' fallito."
::fsbox::mc::CopyFailed						"Copia del file '%s' fallita: permesso negato."
::fsbox::mc::CannotCopy						"Non posso creare una copia perché il file '%s' esiste già."
::fsbox::mc::CannotDuplicate				"Impossibile duplicare il file '%s' per mancanza di permessi lettura."
::fsbox::mc::ReallyDuplicateFile			"Vuoi davvero duplicare questo file?"
::fsbox::mc::ReallyDuplicateDetail		"Questo file ha circa %s. Duplicarlo può richiedere del tempo."
::fsbox::mc::InvalidFileExt				"Operazione fallita: '%s' ha un'estensione file non valida."
::fsbox::mc::CannotRename					"Impossibile rinominare in '%s' perché questa cartella\file esiste già."
::fsbox::mc::CannotCreate					"Non posso creare la cartella '%s' perché questa cartella\file esiste già."
::fsbox::mc::ErrorCreate					"Errore nel creare la cartella: permesso negato."
::fsbox::mc::FilenameNotAllowed			"Il nome del file '%s' non è permesso."
::fsbox::mc::ContainsTwoDots				"Contiene due punti consecutivi."
::fsbox::mc::ContainsReservedChars		"Contiene caratteri riservati: %s, o un carattere di controllo (ASCII 0-31)."
::fsbox::mc::InvalidFileName				"Il nome del file non può iniziare con un trattino (-) e non può finire con uno spazio o un punto."
::fsbox::mc::IsReservedName				"In alcuni sistemi operativi questo è un nome riservato."
::fsbox::mc::FilenameTooLong				"Il nome del file deve avere meno di 256 caratteri."
::fsbox::mc::InvalidFileExtension		"Estensione file '%s' non valida."
::fsbox::mc::MissingFileExtension		"Manca estensione del file in '%s'."
::fsbox::mc::FileAlreadyExists			"Il file '%s' esiste già.\n\nVuoi sovrascriverlo?"
::fsbox::mc::CannotOverwriteDirectory	"Impossibile rinominare la cartella '%s'."
::fsbox::mc::FileDoesNotExist				"Il file '%s' non esiste."
::fsbox::mc::DirectoryDoesNotExist		"La cartella '%s' non esiste."
::fsbox::mc::CannotOpenOrCreate			"Non posso aprire/creare '%s'. Per favore scegli una cartella."
::fsbox::mc::WaitWhileDuplicating		"Per favore attendere mentre avviene la duplicazione del file..."
::fsbox::mc::FileHasDisappeared			"Il file '%s' è scomparso."
::fsbox::mc::CurrentlyInUse				"Questo file è correntemente in uso."
::fsbox::mc::PermissionDenied				"Permessi negati per la cartella '%s'."
::fsbox::mc::CannotOpenUri					"Impossibile aprire il seguente URI:"
::fsbox::mc::InvalidUri						"Il contenuto trascinato non è una lista valida di URI."
::fsbox::mc::UriRejected					"I seguenti file sono rifiutati:"
::fsbox::mc::UriRejectedDetail			"Solo i seguenti tipi di file possono essere gestiti."
::fsbox::mc::CannotOpenRemoteFiles		"Impossibile aprire file remoti:"
::fsbox::mc::CannotCopyFolders			"Impossibile copiare le cartelle. Questa cartelle saranno rifiutate: "
::fsbox::mc::OperationAborted				"Operazione interrotta."
::fsbox::mc::ApplyOnDirectories			"Sei sicuro di voler appllicare l'operazione selezionata sulle (seguenti) cartelle?"
::fsbox::mc::EntryAlreadyExists			"Valore già esistente"
::fsbox::mc::AnEntryAlreadyExists		"Un valore '%s' è già presente."
::fsbox::mc::SourceDirectoryIs			"La cartella d'origine è '%s'."
::fsbox::mc::NewName							"Nuovo nome"
::fsbox::mc::BookmarkAlreadyExists		"Un segnalibro per questa cartella esiste già: '%s'."
::fsbox::mc::AddBookmarkAnyway			"Aggiungi comunque il segnalibro?"
::fsbox::mc::OriginalPathDoesNotExist	"La cartella originale '%s' di questo elemento non esiste più. Creare questa cartella e continuare con l'operazione?"
::fsbox::mc::DragItemAnywhere				"Un'alternativa può essere di trascinare l'elemento in un'altra parte per recuperarlo."

::fsbox::mc::ReallyMove(file,w)			"Vuoi davvero spostare il file '%s' nel cestino?"
::fsbox::mc::ReallyMove(file,r)			"Vuoi davvero spostare il file protetto in scrittura '%s' nel cestino?"
::fsbox::mc::ReallyMove(folder,w)		"Vuoi davvero spostare la cartella '%s' nel cestino?"
::fsbox::mc::ReallyMove(folder,r)		"Vuoi davvero spostare la cartella protetta in scrittura '%s' nel cestino?"
::fsbox::mc::ReallyDelete(file,w)		"Vuoi davvero eliminare il file '%s'? L'operazione è permanente."
::fsbox::mc::ReallyDelete(file,r)		"Vuoi davvero eliminare il file protetto in scrittura '%s'? L'operazione è permanente."
::fsbox::mc::ReallyDelete(link,w)		"Vuoi davvero eliminare il collegamento a '%s'?"
::fsbox::mc::ReallyDelete(link,r)		"Vuoi davvero eliminare il collegamento a '%s'?"
::fsbox::mc::ReallyDelete(folder,w)		"Vuoi davvero eliminare la cartella '%s'? L'operazione è permanente."
::fsbox::mc::ReallyDelete(folder,r)		"Vuoi davvero eliminare la cartella protetta in scrittura '%s'? L'operazione è permanente."
::fsbox::mc::ReallyDelete(empty,w)		"Vuoi veramente eliminare la cartella vuota '%s'? L'operazione è permanente."
::fsbox::mc::ReallyDelete(empty,r)		"Vuoi veramente eliminare la cartella protetta in scrittura '%s'? L'operazione è permanente."

::fsbox::mc::ErrorRenaming(folder)		"Errore nel rinominare la cartella '%old' a '%new': permesso negato."
::fsbox::mc::ErrorRenaming(file)			"Errore nel rinominare il file '%old' a '%new': permesso negato."

::fsbox::mc::Cannot(delete)				"Impossibile eliminare il file '%s'."
::fsbox::mc::Cannot(rename)				"Impossibile rinominare il file '%s'."
::fsbox::mc::Cannot(move)					"Impossibile spostare il file '%s'."
::fsbox::mc::Cannot(overwrite)			"Impossibile sovrascrivere il file '%s'."

::fsbox::mc::DropAction(move)				"Sposta qui"
::fsbox::mc::DropAction(copy)				"Copia qui"
::fsbox::mc::DropAction(link)				"Collega qui"
::fsbox::mc::DropAction(restore)			"Recupera Qui"

### toolbar ############################################################
::toolbar::mc::Toolbar		"Barra strumenti"
::toolbar::mc::Orientation	"Orientamento"
::toolbar::mc::Alignment	"Allineamento"
::toolbar::mc::IconSize		"Grandezza icone"

::toolbar::mc::Default		"Default"
::toolbar::mc::Small			"Piccolo"
::toolbar::mc::Medium		"Medio"
::toolbar::mc::Large			"Grande"

::toolbar::mc::Top			"In alto"
::toolbar::mc::Bottom		"In basso"
::toolbar::mc::Left			"Sinistra"
::toolbar::mc::Right			"Destra"
::toolbar::mc::Center		"Centro"

::toolbar::mc::Flat			"Piatto"
::toolbar::mc::Floating		"Volante"
::toolbar::mc::Hide			"Nascondi"

::toolbar::mc::Expand		"Espandi"

### Countries ##########################################################
::country::mc::Afghanistan											"Afghanistan"
::country::mc::Netherlands_Antilles								"Antille Olandesi"
::country::mc::Anguilla												"Anguilla"
::country::mc::Aboard_Aircraft									"Velivolo Aereo"
::country::mc::Aaland_Islands										"Isole Åland"
::country::mc::Albania												"Albania"
::country::mc::Algeria												"Algeria"
::country::mc::Andorra												"Andorra"
::country::mc::Angola												"Angola"
::country::mc::Antigua												"Antigua e Barbuda"
::country::mc::Australasia											"Australasia"
::country::mc::Argentina											"Argentina"
::country::mc::Armenia												"Armenia"
::country::mc::Aruba													"Aruba"
::country::mc::American_Samoa										"Samoa Americane"
::country::mc::Antarctica											"Antartide"
::country::mc::French_Southern_Territories					"Territori Francesi Meridionali"
::country::mc::Australia											"Australia"
::country::mc::Austria												"Austria"
::country::mc::Azerbaijan											"Azerbaijan"
::country::mc::Bahamas												"Bahamas"
::country::mc::Bangladesh											"Bangladesh"
::country::mc::Barbados												"Barbados"
::country::mc::Basque												"Paesi Baschi"
::country::mc::Burundi												"Burundi"
::country::mc::Belgium												"Belgio"
::country::mc::Benin													"Benin"
::country::mc::Bermuda												"Bermuda"
::country::mc::Bhutan												"Bhutan"
::country::mc::Bosnia_and_Herzegovina							"Bosnia-Erzegovina"
::country::mc::Belize												"Belize"
::country::mc::Belarus												"Bielorussia"
::country::mc::Bolivia												"Bolivia"
::country::mc::Brazil												"Brazile"
::country::mc::Bahrain												"Bahrain"
::country::mc::Brunei												"Brunei"
::country::mc::Botswana												"Botswana"
::country::mc::Bulgaria												"Bulgaria"
::country::mc::Burkina_Faso										"Burkina Faso"
::country::mc::Bouvet_Islands										"Isola Bouvet"
::country::mc::Central_African_Republic						"Repubblica Centraficana"
::country::mc::Cambodia												"Cambogia"
::country::mc::Canada												"Canada"
::country::mc::Catalonia											"Catalonia"
::country::mc::Cayman_Islands										"Isola Cayman"
::country::mc::Cocos_Islands										"Isole Cocos"
::country::mc::Congo													"Congo-Brazzaville"
::country::mc::Chad													"Chad"
::country::mc::Chile													"Cile"
::country::mc::China													"Cina"
::country::mc::Ivory_Coast											"Costa d'Avorio"
::country::mc::Cameroon												"Cameroon"
::country::mc::DR_Congo												"Repubblica Democratica del Congo"
::country::mc::Cook_Islands										"Isole Cook"
::country::mc::Colombia												"Colombia"
::country::mc::Comoros												"Comore"
::country::mc::Cape_Verde											"Capo Verde"
::country::mc::Costa_Rica											"Costa Rica"
::country::mc::Croatia												"Croazia"
::country::mc::Cuba													"Cuba"
::country::mc::Christmas_Island									"Isola Christmas"
::country::mc::Cyprus												"Cipro"
::country::mc::Czech_Republic										"Repubblica Ceca"
::country::mc::Denmark												"Danimarca"
::country::mc::Djibouti												"Djibouti"
::country::mc::Dominica												"Dominica"
::country::mc::Dominican_Republic								"Repubblica Dominicana"
::country::mc::Ecuador												"Ecuador"
::country::mc::Egypt													"Egitto"
::country::mc::England												"Inghilterra"
::country::mc::Eritrea												"Eritrea"
::country::mc::El_Salvador											"El Salvador"
::country::mc::Western_Sahara										"Sahara Occidentale"
::country::mc::Spain													"Spagna"
::country::mc::Estonia												"Estonia"
::country::mc::Ethiopia												"Etiopia"
::country::mc::Faroe_Islands										"Isole Faroe"
::country::mc::Fiji													"Fiji"
::country::mc::Finland												"Finlandia"
::country::mc::Falkland_Islands									"Isole Falkland"
::country::mc::France												"Francia"
::country::mc::West_Germany										"Germania dell'Ovest"
::country::mc::Micronesia											"Micronesia"
::country::mc::Gabon													"Gabon"
::country::mc::Gambia												"Gambia"
::country::mc::Great_Britain										"Gran Bretagna"
::country::mc::Guinea_Bissau										"Guinea-Bissau"
::country::mc::Gibraltar											"Gibilterra"
::country::mc::Guernsey												"Guernsey"
::country::mc::East_Germany										"Germania dell'Est"
::country::mc::Georgia												"Georgia"
::country::mc::Equatorial_Guinea									"Guinea Equatoriale"
::country::mc::Germany												"Germania"
::country::mc::Ghana													"Ghana"
::country::mc::Guadeloupe											"Guadalupa"
::country::mc::Greece												"Grecia"
::country::mc::Grenada												"Granada"
::country::mc::Greenland											"Groenlandia"
::country::mc::Guatemala											"Guatemala"
::country::mc::French_Guiana										"Guyana Francese"
::country::mc::Guinea												"Guinea"
::country::mc::Guam													"Guam"
::country::mc::Guyana												"Guyana"
::country::mc::Haiti													"Haiti"
::country::mc::Hong_Kong											"Hong Kong"
::country::mc::Heard_Island_and_McDonald_Islands			"Isole Heard e McDonald"
::country::mc::Honduras												"Honduras"
::country::mc::Hungary												"Ungheria"
::country::mc::Isle_of_Man											"Isola di Man"
::country::mc::Indonesia											"Indonesia"
::country::mc::India													"India"
::country::mc::British_Indian_Ocean_Territory				"Territorio britannico dell'oceano Indiano"
::country::mc::Iran													"Iran"
::country::mc::Ireland												"Irlanda"
::country::mc::Iraq													"Iraq"
::country::mc::Iceland												"Islanda"
::country::mc::Israel												"Israele"
::country::mc::Italy													"Italia"
::country::mc::British_Virgin_Islands							"Isole Vergini Britanniche"
::country::mc::Jamaica												"Giamaica"
::country::mc::Jersey												"Jersey"
::country::mc::Jordan												"Giordania"
::country::mc::Japan													"Giappone"
::country::mc::Kazakhstan											"Kazakhstan"
::country::mc::Kenya													"Kenya"
::country::mc::Kosovo												"Kosovo"
::country::mc::Kyrgyzstan											"Kyrgyzstan"
::country::mc::Kiribati												"Kiribati"
::country::mc::South_Korea											"Corea del Sud"
::country::mc::Saudi_Arabia										"Arabia Saudita"
::country::mc::Kuwait												"Kuwait"
::country::mc::Laos													"Laos"
::country::mc::Latvia												"Lettonia"
::country::mc::Libya													"Libia"
::country::mc::Liberia												"Liberia"
::country::mc::Saint_Lucia											"Santa Lucia"
::country::mc::Lesotho												"Lesotho"
::country::mc::Lebanon												"Libano"
::country::mc::Liechtenstein										"Liechtenstein"
::country::mc::Lithuania											"Lituania"
::country::mc::Luxembourg											"Lussemburgo"
::country::mc::Macao													"Macao"
::country::mc::Madagascar											"Madagascar"
::country::mc::Morocco												"Marocco"
::country::mc::Malaysia												"Malesia"
::country::mc::Malawi												"Malawi"
::country::mc::Moldova												"Moldavia"
::country::mc::Maldives												"Maldive"
::country::mc::Mexico												"Messico"
::country::mc::Mongolia												"Mongolia"
::country::mc::Marshall_Islands									"Isole Marshall"
::country::mc::Macedonia											"Macedonia"
::country::mc::Mali													"Mali"
::country::mc::Malta													"Malta"
::country::mc::Montenegro											"Montenegro"
::country::mc::Northern_Mariana_Islands						"Isole Marianne Settentrionali"
::country::mc::Monaco												"Monaco"
::country::mc::Mozambique											"Mozambico"
::country::mc::Mauritius											"Mauritius"
::country::mc::Montserrat											"Montserrat"
::country::mc::Mauritania											"Mauritania"
::country::mc::Martinique											"Martinica"
::country::mc::Myanmar												"Myanmar"
::country::mc::Mayotte												"Mayotte"
::country::mc::Namibia												"Namibia"
::country::mc::Nicaragua											"Nicaragua"
::country::mc::New_Caledonia										"Nuova Caledonia"
::country::mc::Netherlands											"Olanda"
::country::mc::Nepal													"Nepal"
::country::mc::The_Internet										"Internet"
::country::mc::Norfolk_Island										"Isole Norfolk"
::country::mc::Nigeria												"Nigeria"
::country::mc::Niger													"Niger"
::country::mc::Northern_Ireland									"Irlanda del Nord"
::country::mc::Niue													"Niue"
::country::mc::Norway												"Norvegia"
::country::mc::Nauru													"Nauru"
::country::mc::New_Zealand											"Nuova Zelanda"
::country::mc::Oman													"Oman"
::country::mc::Pakistan												"Pakistan"
::country::mc::Panama												"Panama"
::country::mc::Paraguay												"Paraguay"
::country::mc::Pitcairn_Islands									"Isole Pitcairn"
::country::mc::Peru													"Perù"
::country::mc::Philippines											"Filippine"
::country::mc::Palestine											"Palestina"
::country::mc::Palau													"Palau"
::country::mc::Papua_New_Guinea									"Papua Nuova Guinea"
::country::mc::Poland												"Polonia"
::country::mc::Portugal												"Portogallo"
::country::mc::North_Korea											"Corea del Nord"
::country::mc::Puerto_Rico											"Porto Rico"
::country::mc::French_Polynesia									"Polinesia Francese"
::country::mc::Qatar													"Qatar"
::country::mc::Reunion												"Réunion"
::country::mc::Romania												"Romania"
::country::mc::South_Africa										"Sudafrica"
::country::mc::Russia												"Russia"
::country::mc::Rwanda												"Rwanda"
::country::mc::Samoa													"Samoa"
::country::mc::Serbia_and_Montenegro							"Serbia e Montenegro"
::country::mc::Scotland												"Scozia"
::country::mc::At_Sea												"In mare"
::country::mc::Senegal												"Senegal"
::country::mc::Seychelles											"Seychelles"
::country::mc::South_Georgia_and_South_Sandwich_Islands	"Georgia del Sud e isole Sandwich meridionali"
::country::mc::Saint_Helena										"Sant'Elena"
::country::mc::Singapore											"Singapore"
::country::mc::Jan_Mayen_and_Svalbard							"Svalbard e Jan Mayen"
::country::mc::Saint_Kitts_and_Nevis							"Saint Kitts e Nevis"
::country::mc::Sierra_Leone										"Sierra Leone"
::country::mc::Slovenia												"Slovenia"
::country::mc::San_Marino											"San Marino"
::country::mc::Solomon_Islands									"Isole Salomone"
::country::mc::Somalia												"Somalia"
::country::mc::Aboard_Spacecraft									"Stazione Spaziale Internazionale"
::country::mc::Saint_Pierre_and_Miquelon						"Saint-Pierre e Miquelon"
::country::mc::Serbia												"Serbia"
::country::mc::Sri_Lanka											"Sri Lanka"
::country::mc::Sao_Tome_and_Principe							"Sao Tome e Principe"
::country::mc::Sudan													"Sudan"
::country::mc::Switzerland											"Svizzera"
::country::mc::Suriname												"Suriname"
::country::mc::Slovakia												"Slovacchia"
::country::mc::Sweden												"Svezia"
::country::mc::Swaziland											"Swaziland"
::country::mc::Syria													"Siria"
::country::mc::Tanzania												"Tanzania"
::country::mc::Turks_and_Caicos_Islands						"Isole Turks e Caicos"
::country::mc::Czechoslovakia										"Cecoslovacchia"
::country::mc::Tonga													"Tonga"
::country::mc::Thailand												"Tailandia"
::country::mc::Tibet													"Tibet"
::country::mc::Tajikistan											"Tajikistan"
::country::mc::Tokelau												"Tokelau"
::country::mc::Turkmenistan										"Turkmenistan"
::country::mc::Timor_Leste											"Timor Est"
::country::mc::Togo													"Togo"
::country::mc::Chinese_Taipei										"Taiwan"
::country::mc::Trinidad_and_Tobago								"Trinidad e Tobago"
::country::mc::Tunisia												"Tunisia"
::country::mc::Turkey												"Turchia"
::country::mc::Tuvalu												"Tuvalu"
::country::mc::United_Arab_Emirates								"Emirati Arabi Uniti"
::country::mc::Uganda												"Uganda"
::country::mc::Ukraine												"Ucraina"
::country::mc::United_States_Minor_Outlying_Islands		"Isole minori esterne degli Stati Uniti d'America"
::country::mc::Unknown												"(Sconosciuta)"
::country::mc::Soviet_Union										"Unione Sovietica"
::country::mc::Uruguay												"Uruguay"
::country::mc::United_States_of_America						"Stati Uniti d'America"
::country::mc::Uzbekistan											"Uzbekistan"
::country::mc::Vanuatu												"Vanuatu"
::country::mc::Vatican												"Città del Vaticano"
::country::mc::Venezuela											"Venezuela"
::country::mc::Vietnam												"Vietnam"
::country::mc::Saint_Vincent_and_the_Grenadines				"Saint Vincent e Grenadines"
::country::mc::US_Virgin_Islands									"Isole Vergini Americane"
::country::mc::Wallis_and_Futuna									"Wallis e Futuna"
::country::mc::Wales													"Galles"
::country::mc::Yemen													"Yemen"
::country::mc::Yugoslavia											"Yugoslavia"
::country::mc::Zambia												"Zambia"
::country::mc::Zanzibar												"Zanzibar"
::country::mc::Zimbabwe												"Zimbabwe"
::country::mc::Mixed_Team											"Squadra mista"

::country::mc::Africa_North										"Africa, Nord"
::country::mc::Africa_Sub_Saharan								"Africa, Sub-Sahara"
::country::mc::America_Caribbean									"America, Caraibi"
::country::mc::America_Central									"America, Centrale"
::country::mc::America_North										"America, Nord"
::country::mc::America_South										"America, Sud"
::country::mc::Antarctic											"Antartide"
::country::mc::Asia_East											"Asia, Est"
::country::mc::Asia_South_South_East							"Asia, Sud-Est"
::country::mc::Asia_West_Central									"Asia, Ovest-Centale"
::country::mc::Europe												"Europa"
::country::mc::Europe_East											"Europa, Est"
::country::mc::Oceania												"Oceania"
::country::mc::Stateless											"Apolide"

### Languages ##########################################################
::encoding::mc::Lang(FI)	"Fide"
::encoding::mc::Lang(af)	"Afrikaans"
::encoding::mc::Lang(ar)	"Arabo"
::encoding::mc::Lang(ast)	"Leonese"
::encoding::mc::Lang(az)	"Azero"
::encoding::mc::Lang(bat)	"Baltico"
::encoding::mc::Lang(be)	"Bielorusso"
::encoding::mc::Lang(bg)	"Bulgaro"
::encoding::mc::Lang(br)	"Bretone"
::encoding::mc::Lang(bs)	"Bosniaco"
::encoding::mc::Lang(ca)	"Catalano"
::encoding::mc::Lang(cs)	"Ceco"
::encoding::mc::Lang(cy)	"Gallese"
::encoding::mc::Lang(da)	"Danese"
::encoding::mc::Lang(de)	"Tedesco"
::encoding::mc::Lang(de+)	"Tedesco (riformato)"
::encoding::mc::Lang(el)	"Greco"
::encoding::mc::Lang(en)	"Inglese"
::encoding::mc::Lang(eo)	"Esperanto"
::encoding::mc::Lang(es)	"Spagnolo"
::encoding::mc::Lang(et)	"Estone"
::encoding::mc::Lang(eu)	"Basco"
::encoding::mc::Lang(fi)	"Finlandese"
::encoding::mc::Lang(fo)	"Faroese"
::encoding::mc::Lang(fr)	"Francese"
::encoding::mc::Lang(fy)	"Frisico"
::encoding::mc::Lang(ga)	"Irlandese"
::encoding::mc::Lang(gd)	"Scozzese"
::encoding::mc::Lang(gl)	"Gaelico"
::encoding::mc::Lang(he)	"Ebreo"
::encoding::mc::Lang(hi)	"Hindi"
::encoding::mc::Lang(hr)	"Croato"
::encoding::mc::Lang(hu)	"Ungherese"
::encoding::mc::Lang(hy)	"Armeno"
::encoding::mc::Lang(ia)	"Interlingua"
::encoding::mc::Lang(id)	"Indonesiano"
::encoding::mc::Lang(is)	"Islandese"
::encoding::mc::Lang(it)	"Italiano"
::encoding::mc::Lang(iu)	"Inuktitut"
::encoding::mc::Lang(ja)	"Giapponese"
::encoding::mc::Lang(ka)	"Georgiano"
::encoding::mc::Lang(kk)	"Kazaka"
::encoding::mc::Lang(kl)	"Groenlandese"
::encoding::mc::Lang(ko)	"Coreano"
::encoding::mc::Lang(ku)	"Curdo"
::encoding::mc::Lang(ky)	"Kyrgyz"
::encoding::mc::Lang(la)	"Latino"
::encoding::mc::Lang(lb)	"Lussemburghese"
::encoding::mc::Lang(lt)	"Lituano"
::encoding::mc::Lang(lv)	"Lettone"
::encoding::mc::Lang(mk)	"Macedone"
::encoding::mc::Lang(mo)	"Moldavo"
::encoding::mc::Lang(ms)	"Malese"
::encoding::mc::Lang(mt)	"Maltese"
::encoding::mc::Lang(nl)	"Olandese"
::encoding::mc::Lang(no)	"Norvegese"
::encoding::mc::Lang(oc)	"Occitano"
::encoding::mc::Lang(pl)	"Polacco"
::encoding::mc::Lang(pt)	"Portoghese"
::encoding::mc::Lang(rm)	"Romancio"
::encoding::mc::Lang(ro)	"Rumeno"
::encoding::mc::Lang(ru)	"Russo"
::encoding::mc::Lang(se)	"Sami"
::encoding::mc::Lang(sk)	"Slovacco"
::encoding::mc::Lang(sl)	"Sloveno"
::encoding::mc::Lang(sq)	"Albanese"
::encoding::mc::Lang(sr)	"Serbo"
::encoding::mc::Lang(sv)	"Svedese"
::encoding::mc::Lang(sw)	"Swahili"
::encoding::mc::Lang(tg)	"Tagico"
::encoding::mc::Lang(th)	"Tailandese"
::encoding::mc::Lang(tk)	"Turkmena"
::encoding::mc::Lang(tl)	"Tagalog"
::encoding::mc::Lang(tr)	"Turco"
::encoding::mc::Lang(uk)	"Ucraino"
::encoding::mc::Lang(uz)	"Usbeco"
::encoding::mc::Lang(vi)	"Vietnamese"
::encoding::mc::Lang(wa)	"Vallone"
::encoding::mc::Lang(wen)	"Lusaziano"
::encoding::mc::Lang(hsb)	"Alto Sorbo" 
::encoding::mc::Lang(dsb)	"Basso Sorbo"
::encoding::mc::Lang(zh)	"Cinese"

::encoding::mc::Font(hi)	"Devanagari"

### Calendar ###########################################################
::calendar::mc::OneMonthForward	"Un mese avanti (Shift \u2192)"
::calendar::mc::OneMonthBackward	"Un mese indietro (Shift \u2190)"
::calendar::mc::OneYearForward	"Un anno avanti (Ctrl \u2192)"
::calendar::mc::OneYearBackward	"Un anno indietro (Ctrl \u2190)"

::calendar::mc::Su	"Do"
::calendar::mc::Mo	"Lu"
::calendar::mc::Tu	"Ma"
::calendar::mc::We	"Me"
::calendar::mc::Th	"Gi"
::calendar::mc::Fr	"Ve"
::calendar::mc::Sa	"Sa"

::calendar::mc::Jan	"Gen"
::calendar::mc::Feb	"Feb"
::calendar::mc::Mar	"Mar"
::calendar::mc::Apr	"Apr"
::calendar::mc::May	"Mag"
::calendar::mc::Jun	"Giu"
::calendar::mc::Jul	"Lug"
::calendar::mc::Aug	"Ago"
::calendar::mc::Sep	"Set"
::calendar::mc::Oct	"Ott"
::calendar::mc::Nov	"Nov"
::calendar::mc::Dec	"Dic"

::calendar::mc::MonthName(1)		"Gennaio"
::calendar::mc::MonthName(2)		"Febbraio"
::calendar::mc::MonthName(3)		"Marzo"
::calendar::mc::MonthName(4)		"Aprile"
::calendar::mc::MonthName(5)		"Maggio"
::calendar::mc::MonthName(6)		"Giugno"
::calendar::mc::MonthName(7)		"Luglio"
::calendar::mc::MonthName(8)		"Agosto"
::calendar::mc::MonthName(9)		"Settembre"
::calendar::mc::MonthName(10)		"Ottobre"
::calendar::mc::MonthName(11)		"Novembre"
::calendar::mc::MonthName(12)		"Dicembre"

::calendar::mc::WeekdayName(0)	"Domenica"
::calendar::mc::WeekdayName(1)	"Lunedì"
::calendar::mc::WeekdayName(2)	"Martedì"
::calendar::mc::WeekdayName(3)	"Mercoledì"
::calendar::mc::WeekdayName(4)	"Giovedì"
::calendar::mc::WeekdayName(5)	"Venerdì"
::calendar::mc::WeekdayName(6)	"Sabato"

### emoticons ##########################################################
::emoticons::mc::Tooltip(smile)		"Sorriso"
::emoticons::mc::Tooltip(frown)		"Cipiglio"
::emoticons::mc::Tooltip(saint)		"Santo"
::emoticons::mc::Tooltip(evil)		"Male"
::emoticons::mc::Tooltip(gleeful)	"Gioioso"
::emoticons::mc::Tooltip(wink)		"Occhiolino"
::emoticons::mc::Tooltip(cool)		"Fico"
::emoticons::mc::Tooltip(grin)		"Ghigno"
::emoticons::mc::Tooltip(neutral)	"Neutrale"
::emoticons::mc::Tooltip(sweat)		"Sudando"
::emoticons::mc::Tooltip(confuse)	"Confuso"
::emoticons::mc::Tooltip(shock)		"Scioccato"
::emoticons::mc::Tooltip(kiss)		"Bacio"
::emoticons::mc::Tooltip(razz)		"Incredulo"
::emoticons::mc::Tooltip(grumpy)		"Scontroso"
::emoticons::mc::Tooltip(upset)		"Agitato"
::emoticons::mc::Tooltip(cry)			"Pianto"
::emoticons::mc::Tooltip(yell)		"Urla"
::emoticons::mc::Tooltip(surprise)	"Sorpreso"
::emoticons::mc::Tooltip(red)			"Vergogna"
::emoticons::mc::Tooltip(sleep)		"Sonnolento"
::emoticons::mc::Tooltip(eek)			"Spaventato"
::emoticons::mc::Tooltip(kitty)		"Micio"
::emoticons::mc::Tooltip(roll)		"Girare gli occhi"
::emoticons::mc::Tooltip(blink)		"Sbattere le palpebre"
::emoticons::mc::Tooltip(glasses)	"Intelligente"

### remote #############################################################
::remote::mc::PostponedMessage "Apertura database \"%s\" in pausa fino a che l'operazione attuale non è conclusa."

### web ################################################################
::web::mc::SaveFile "Save File" ;# NEW

# vi:set ts=3 sw=3:
