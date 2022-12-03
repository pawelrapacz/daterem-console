![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/pawelrapacz/daterem-console)
![GitHub](https://img.shields.io/github/license/pawelrapacz/daterem-console?color=greem)
# daterem

> Helps you keep track your plans and reminds you your daily things.

## Table of Contents
* [General Information](#general-information)
* [Features](#features)
* [Setup](#setup)
* [Usage](#usage)
* [Project Status](#project-status)
* [Upcoming changes](#upcoming-changes)
* [Contact](#contact)
* [License](#license)


## General Information
This windows terminal app was made to help you with keeping track of your plans and remaind you about anything.
Just with using a few commands you can create and manage reminders that will help you memorise anything you want so that you won't need to worry about missing out on your plans.
You can create reminders that pop up once in the specified day or ones that will remind you a week and two days before.
Reminders can also be annulal or set with specified year.

Why creating such project?<br>
I undertook this project mainly to inprove my coding skills, learn something new and of course have some fun while coding! 

<!-- 
## Technologies Used
- C++ -->


## Features
- Reminds you anything
- Easy to use
- Multiple reminding



## Setup
You can install daterem by downloading the package and adding it to your [Path](https://gist.github.com/nex3/c395b2f8fd4b02068be37c961301caa7).


## Usage

### Creating new remineder
After you have installed the program you can create your first reminder by using:

<pre><code>daterem --new 01.01 "some awesome title" "some awesome description"</code></pre>

The command above creates new annual reminder for specified date, what is important both title and description need to be specified.
In case you want to create one time reminder you just specify the year e.g. `01.01.2022`.

### View existing reminders
Now when you have your first reminder created you can see it by using `--list` option.
<pre><code>daterem --list</code></pre>
<pre>
1 - 01.01 - some awesome title - some awesome description 
2 - 01.01.2022 - some awesome title - some awesome description
</pre>

### Check reminders for today
To see reminedrs scheduled for today just use `daterem` or:
<pre><code>daterem --check</code></pre>
<pre>
[01.01.2022]

some awesome title some awesome description
some awesome title some awesome description
</pre>

### Deleting reminders
You can delete specified reminder by using:
<pre><code>daterem --delete 1</code></pre>
This commad deleted reminder that has number 1, now when you list our reminders you can see that the first one was deleted.
<pre>
C:> daterem --list
1 - 01.01.2022 - some awesome title - some awesome description
</pre>

### Outdated reminders
Outdated reminders aren't deleted automaticly so you have to do it yourself, esspecially when you have many reminders scheduled it can get quite messy and difficult to manage your reminders. Deleting every outdated reminder individually would be really problematic. So to remove all outdated reminders use:
<pre><code>daterem --delete-outdated</code></pre>
<pre>
01.01.2022 - some awesome title - some awesome description
Deleted: 1 reminders
</pre>
Of course this option applies to one time reminders only.

### Additional reminders
You can create reminders that will additionally remind you week and two days before specifed date. To create such one use `-b` option:
<pre><code>daterem -b --new 01.01 "some awesome title" "some awesome description"</code></pre>
Additional reminding can be added to existing reminders, just specify the number of the reminder:
<pre><code>daterem -b 1</code></pre>


### Converting reminders
Some day you might want to change one time reminder to annual one, then you can use `-e` option:
<pre><code>daterem -e 1</code></pre>
This option can also be used with `--new`, when used this way forces the reminder to be annual:
<pre><code>daterem -e --new 01.01.2022 "some awesome title" "some awesome description"</code></pre>
Despite the year specified created reminder is annual. You can see it after using `--list` option.
<pre>
C:> daterem --list
1 - 01.01 - some awesome title - some awesome description 
</pre>


## Project Status
Project is still in progress, I am adding new features frequently, if you have any propositions for new features or you found a bug you can contact me [here](#contact).


## Upcoming changes

### Things that need improvement:


### Upcoming features:
- Weekly reminders
- Every day reminders
- Reminding on startup



## Contact
Created by [@pawelrapacz](https://github.com/pawelrapacz/) - feel free to contact me!



## License
This project is open source and available under the GNU GPL [License](LICENSE).<br>
