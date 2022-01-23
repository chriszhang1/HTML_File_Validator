import System.IO
import Data.Char

data Token = Html|Head|Body|Title|H1|H2|H3|P|Ul|Li|A|Div deriving(Show, Eq)

data FullToken = O Token|C Token|Br|Hr|Dummy|Error deriving(Show, Eq)

data Result = InvalidTagPresent|MissingOpeningHtmlTag|MissingOpeningHeadTag|MissingOpeningTitleTag|MissingOpeningBodyTag|MissingClosingTag|ValidFile|InvalidFile|POrDivNestedInsideP|TitleInBody deriving(Show)

main :: IO ()
main = do
    handle <- openFile "file.html" ReadMode
    contents <- hGetContents handle
    let tokens = tokenize contents in print(checkAll tokens [])
    hClose handle 

extractHead :: [Char] -> Char -> [Char]
extractHead [] _ = []
extractHead (x:xs) char 
    | char `notElem` (x:xs) = x:xs
    | head xs == char = [x] 
    | char `elem` xs = x : extractHead xs char
    | otherwise = x:xs

extractTail :: [Char] -> Char -> [Char]
extractTail [] _ = []
extractTail (x:xs) char 
    | x == char = x:xs 
    | char `elem` xs = extractTail xs char
    | otherwise = []

strToToken :: [Char] -> FullToken
strToToken "html" = O Html
strToToken "/html" = C Html
strToToken "head" = O Head
strToToken "/head" = C Head
strToToken "body" = O Body
strToToken "/body" = C Body
strToToken "title" = O Title
strToToken "/title" = C Title
strToToken "h1" = O H1
strToToken "/h1" = C H1
strToToken "h2" = O H2
strToToken "/h2" = C H2
strToToken "h3" = O H3
strToToken "/h3" = C H3
strToToken "p" = O P
strToToken "/p" = C P
strToToken "ul" = O Ul
strToToken "/ul" = C Ul
strToToken "li" = O Li
strToToken "/li" = C Li
strToToken "a" = O A
strToToken "/a" = C A
strToToken "div" = O Div
strToToken "/div" = C Div
strToToken "br" = Br
strToToken "hr" = Hr
strToToken s = Dummy

tokenize :: [Char] -> [FullToken]
tokenize list
    | null list = []
    | isSpace x = tokenize xs
    | x == '<' = strToToken tag : tokenize remain 
    | otherwise = if extractTail list '<' == "" && extractTail list '>' == "" then [Dummy]
                else if extractTail list '<' /= "" then Dummy : tokenize (extractTail list '<') 
                else [Error]

    
    where tag = extractHead fullTag ' '; remain = tail (extractTail xs '>'); fullTag = extractHead xs '>'; x = head list; xs = tail list

isEmpty :: (Eq a) => [a] -> Bool
isEmpty xs
    | null xs = True
    | otherwise = False

isMatched :: FullToken -> FullToken -> Bool
isMatched (O x) (C y) = x == y
isMatched _ _ = False

checkAll :: [FullToken] -> [FullToken] -> Result
checkAll [] [] = MissingOpeningHtmlTag
checkAll xs [] = if Error `elem` xs then InvalidTagPresent else checkHtml xs []

checkHtml :: [FullToken] -> [FullToken] -> Result
checkHtml (x:xs) [] = if x /= O Html then MissingOpeningHtmlTag else checkHead xs [x]

checkHead :: [FullToken] -> [FullToken] -> Result
checkHead (x:xs) ys
    | x /= O Head = MissingOpeningHeadTag 
    | head xs /= C Head = checkToken (x:xs) ys
    | otherwise = MissingOpeningTitleTag

checkBody :: [FullToken] -> [FullToken] -> Result
checkBody (x:xs) ys 
    | x /= O Body = MissingOpeningBodyTag
    | O Title `elem` (x:xs) = TitleInBody
    | otherwise = checkToken (x:xs) ys

checkToken :: [FullToken] -> [FullToken] -> Result
checkToken [] x = if isEmpty x then ValidFile else MissingClosingTag
checkToken (x:xs) [] = if x `elem` [Br, Hr, Dummy] then checkToken xs [] else checkToken xs [x]
checkToken (x:xs) ys 
    | x == C Head = checkBody xs (init ys)
    | last ys == O P && x `elem` [O P, O Div] = POrDivNestedInsideP
    | x `elem` [Br, Hr, Dummy] = checkToken xs ys
    | isMatched (last ys) x = checkToken xs (init ys) 
    | otherwise = checkToken xs (ys++[x])
    