% uist96.cls
%
% Originally developed by Stephane Chatty [chatty@dgac.fr].
% Modifications by Steve Feiner [feiner@cs.columbia.edu] noted with SKF.
% Modifications by Jean-Daniel Fekete [jdf@lri.fr] to turn it into a
%  LaTeX2e class file
%
% The style for UIST'96 submissions is that same as has been used by we've used in the
% past few years. 
%
% uist96.cls is to be used like this:
%
% \documentclass{uist96}
% \usepackage{times}
% \begin{document}
% \title{Your Title}
% \author{Your name and address}
% \maketitle
% \abstract
% Your abstract goes here.
% \keywords Your, keywords, go, here, separated, by, commas
% \section{INTRODUCTION}
% Etc., etc. . . .
% \end{document}
%
% I was not able to redefine the output routine in order to
% make the two columns of the last page equal in length.
% To do that, add a \linebreak (to justify the current line)
% and \newpage (to start a new column)
% at the right place in the text on your last page.
\NeedsTeXFormat{LaTeX2e}[1993/12/01]
\def\fileversion{1.0}
\def\filedate{1996/04/01}
\def\filename{uist96.dtx}
\ProvidesClass{uist96}[\filedate\space Production of UIST'96 Article]
\typeout{Document Class "UIST '96" <\filedate>.}
\newif\if@draft
\newcount\@minutes
\newcount\@seconds
\@draftfalse
\DeclareOption{draft}{\setlength\overfullrule{5pt} \@drafttrue}
\newcommand\parentclass{article}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{\parentclass}}
\ProcessOptions
\LoadClass[twocolumn]{\parentclass}
% SKF: Changed pagestyle default to empty
% change this to \pagestyle{plain} to temporarily add page numbers
\pagestyle{plain}

% slight change to line spacing
\renewcommand\normalsize{%
  \@setsize\normalsize{11pt}\xpt\@xpt
  \abovedisplayskip 10\p@ \@plus2\p@ \@minus5\p@
  \belowdisplayskip \abovedisplayskip
  \abovedisplayshortskip \z@ plus3\p@
  \belowdisplayshortskip 6\p@ \@plus3\p@ \@minus3\p@
  \let\@listi\@listI}
% redefined to avoid parskip after section headings
\def\@xsect#1{\@tempskipa #1\relax
      \ifdim \@tempskipa>\z@
       \par \nobreak
       \vskip \@tempskipa
       \vskip-\parskip % added
       \@afterheading
    \else \global\@nobreakfalse \global\@noskipsectrue
       \everypar{\if@noskipsec \global\@noskipsecfalse
                   \clubpenalty\@M \hskip -\parindent
                   \begingroup \@svsechd \endgroup \unskip
                   \@tempskipa #1\relax
                   \hskip -\@tempskipa
                  \else \clubpenalty \@clubpenalty
                    \everypar{}\fi}\fi\ignorespaces}

% all these are redefined to adjust headings style

\renewcommand\section{\@startsection {section}{1}{\z@}%
                                   {-0.01ex \@plus -.2ex}%
                                   {.01ex \@plus .2ex}%
                                   {\reset@font\small\sffamily\bfseries}}
\renewcommand\subsection{\@startsection{subsection}{2}{\z@}%
                                   {-0.01ex \@plus -.2ex}%
                                   {.01ex \@plus .2ex}%
                                   {\reset@font\small\sffamily\bfseries}}
\renewcommand\subsubsection{\@startsection{subsubsection}{3}{\z@}%
                                   {0.01ex \@plus .2ex}%
                                   {-1em}%
                                   {\reset@font\normalsize\sffamily\slshape}}
\renewcommand\paragraph{\@startsection{paragraph}{4}{\z@}%
                                    {0ex \@plus .2ex}%
                                    {-1em}%
                                    {\reset@font\normalsize\sffamily\bfseries}}
\renewcommand\subparagraph{\@startsection{subparagraph}{5}{\parindent}%
                                       {0ex plus .2ex}%
                                       {-1em}%
                                       {\reset@font\normalsize\sffamily\slshape}}
\setlength\parskip{1.75ex \@plus 1pt}
% indentation of paragraphs
\setlength\parindent{\z@}
% SKF: Section numbers may be optionally used for UIST!
% no section level is numbered
% \setcounter{secnumdepth}{0}
\setcounter{secnumdepth}{2}

% abstract is just another section
\def\abstract{\section*{ABSTRACT}}
% SKF: keywords is a paragraph
\def\keywords{\paragraph{\small KEYWORDS:}}    % SKF

% redefined maketitle to eliminate \thispagestyle{plain} and add the copyright box
\newif\if@copyright
\@copyrighttrue

\def\nocopyright{\@copyrightfalse}
%\def\@makefnmark{\hbox{$^{\@thefnmark}\m@th$}}

\def\maketitle{\par
 \begingroup
 \def\thefootnote{\fnsymbol{footnote}}
 \def\@makefnmark{\hbox to 0pt{$^{\@thefnmark}$\hss}}
 \if@twocolumn
   \twocolumn[\@maketitle]
 \else
   \global\@topnum\z@
   \@maketitle
 \fi
%%% SKF: If you want to disable the copyright box (e.g., for an IEEE
%%% publication), then comment out all lines from here up to the
%%% line labeled ``End of copyright box''
%%%
%%% SKF: NOTE: Assume that \textfloatsep is 20pt.  Therefore height of picture
%%%       must be 2.54cm - 20pts = 1.84cm
%%%

\if@copyright
 \begin{figure}[b]
 \setlength{\unitlength}{1cm}
 \begin{picture}(8.5,1.84) %% SKF: Compensate for \textfloatsep in height!!!
 \end{picture}
 \end{figure}
\fi
%%% End of copyright box
 \@thanks
 \endgroup
 \setcounter{footnote}{0}
 \let\maketitle\relax
 \let\@maketitle\relax
 \gdef\@thanks{}\gdef\@author{}\gdef\@title{}\let\thanks\relax}

% redefined title presentation
\def\@maketitle{
% \vbox to 2.5in{
  \begin{center}
 {\LARGE\sffamily\bfseries\baselineskip 20pt \@title \par}
 \vskip 1.5em
 {\lineskip .5em\large
 \begin{tabular}[t]{c}%
 \@author
 \end{tabular}\par}%
 \end{center}
 \vskip 5ex
%\vfill}
}

% default definition for \author
\def\authorname#1{\gdef\@authorname{#1}}
\gdef\@authorname{Author Name}

\def\authoraddr#1{\gdef\@authoraddr{#1}}
\gdef\@authoraddr{Author Affiliation\\Street Name\\City}

\author{
  \begin{tabular}{c}
  {\em\@authorname}\\[.4cm]
  \@authoraddr
  \end{tabular}
}


\@minutes=\time
\divide\@minutes 60
\@seconds=-\@minutes
\multiply\@seconds 60
\advance\@seconds \time
\edef\@now{\two@digits{\the\@minutes}:\two@digits{\the\@seconds}}

\def\ps@plain{\let\@mkboth\@gobbletwo%
  \let\@oddhead\@empty\let\@evenhead\@empty
  \if@draft
    \def\@oddfoot{\reset@font\rmfamily%
      \hbox to 0pt{\small Draft of \today \space \@now\hss}\hfil
      \thepage\hfil}
  \else
    \def\@oddfoot{\hfil\reset@font\rmfamily\thepage\hfil}
    \let\@evenfoot\@oddfoot
   \fi\let\@evenhead\@oddhead}


% redefinition of itemize  %% SKF: Modified to indent all lines past 1st.
 \def\itemize{\ifnum \@itemdepth >3 \@toodeep\else \advance\@itemdepth \@ne
 \edef\@itemitem{labelitem\romannumeral\the\@itemdepth}%
 \list{\csname\@itemitem\endcsname}{\topsep -.5\parskip\parsep 0pt%
 \itemsep 0pt\itemindent 0pt\labelwidth
1ex\def\makelabel##1{\hss\llap{##1}}}\fi}
 \leftmargini 2ex       %% SKF
 \leftmarginii 2ex      %% SKF


% redefinition of captions
\long\def\@makecaption#1#2{
 \vskip 4pt
 \@tempdima\hsize
  \advance\@tempdima -1cm
 \setbox\@tempboxa\hbox{\sffamily\small #1: #2}
 \ifdim \wd\@tempboxa >\@tempdima
     {\advance\leftskip .5cm \advance\rightskip .5cm\sffamily\small
     \baselineskip 10pt #1: #2\par} % SKF: added baselineskip
  \else
     {\centering \hbox to\@tempdima{\hfil\box\@tempboxa\hfil}}
 \fi}

% redefined bibliography stuff to change the heading and the labels
\def\@biblabel#1{#1.\hfill}

\def\thebibliography#1{\section*{REFERENCES\@mkboth{REFERENCES}{REFERENCES}}
  \list{\arabic{enumi}.}{
    \settowidth\labelwidth{[#1]}
    \leftmargin\labelwidth
    \advance\leftmargin\labelsep
    \topsep\z@
    \parskip\z@
    \partopsep\z@
    \usecounter{enumi}}
 \def\newblock{\hskip .11em plus .33em minus .07em}
 \sloppy
 \clubpenalty4000
 \widowpenalty4000
 \sfcode`\.=1000\relax}

% misc. style parameters
\columnsep .8cm         % SKF: Changed from .85cm
\textheight 23.5cm
\textwidth 17.8cm       % SKF: Changed from 17.85cm
\headheight 0pt
\headsep 0pt
\topmargin 0.5in
\oddsidemargin -.25in
\evensidemargin -.25in
\textfloatsep 20pt      % SKF: Assume 20pts, so copyright box is right size
