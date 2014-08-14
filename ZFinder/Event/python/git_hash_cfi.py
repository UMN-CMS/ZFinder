from commands import getstatusoutput

def git_hash():
    """ Returns a string of the hash of the latest git commit. """
    return getstatusoutput('git rev-parse HEAD')[1]
